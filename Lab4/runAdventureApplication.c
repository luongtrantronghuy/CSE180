/**
 * runAdventureApplication skeleton, to be modified by students
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libpq-fe.h"

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value this big */
#define  MAXNUMBERSTRINGSIZE        20


/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 */

/* Function: printNumberOfThingsInRoom:
 * -------------------------------------
 * Parameters:  connection, and theRoomID, which should be the roomID of a room.
 * Prints theRoomID, and number of things in that room.
 * Return 0 if normal execution, -1 if no such room.
 * bad_exit if SQL statement execution fails.
 */


int printNumberOfThingsInRoom(PGconn *conn, int theRoomID)
{
    char request[MAXCONNECTIONSTRINGSIZE] = "SELECT r.roomDescription, COUNT(thingID) FROM Rooms r, Things t WHERE roomID = ";
    char theRoomIDstr[100];
    sprintf(theRoomIDstr, "%d", theRoomID);
    strcat(request, theRoomIDstr);
    strcat(request, " AND initialRoomID = ");
    strcat(request, theRoomIDstr);
    strcat(request, " GROUP BY r.roomDescription");
    // printf(request, "\n");

    PGresult *res = PQexec(conn, request);
    
    // If there's an error, exit with bad_exit
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf(PQerrorMessage(conn));      
        PQclear(res);
        bad_exit(conn);
        return -1;
    }

    // If no rows were returned, return with -1
    int rows = PQntuples(res);
    if (rows == 0) { 
        printf("RoomID %s not found!\n", theRoomIDstr);
        PQclear(res);
        return -1;
    }

    // Get values from the return row (should only be one)
    char *desc = PQgetvalue(res, 0, 0);
    char *count = PQgetvalue(res, 0, 1);
    printf("Room %s, %s, has %s in it.\n", theRoomIDstr, desc, count);
    
    PQclear(res);
    return 0;
}

/* Function: updateWasDefeated:
 * ----------------------------
 * Parameters:  connection, and a string, doCharactersOrMonsters, which should be 'C' or 'M'.
 * Updates the wasDefeated value of either characters or monsters (depending on value of
 * doCharactersOrMonsters) if that value is not correct.
 * Returns number of characters or monsters whose wasDefeated value was updated,
 * or -1 if doCharactersOrMonsters value is invalid.
 */

int updateWasDefeated(PGconn *conn, char *doCharactersOrMonsters)
{
    int isM = 0;
    int isC = 0;

    if(strcmp(doCharactersOrMonsters, "M") == 0){
        isM = 1;
    }else if(strcmp(doCharactersOrMonsters, "C") == 0){
        isC = 1;
    }else{
        printf("Not M or C\n");
        return -1;
    }

    // Request for Losing Monsters
    char requestLosingM[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT b.monsterID FROM Battles b, Monsters m WHERE b.monsterBattlePoints < b.characterBattlePoints AND m.wasDefeated != True GROUP BY b.monsterID;";
    // Request for Winning Monsters
    char requestWinningM[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT b.monsterID FROM Battles b, Monsters m WHERE b.monsterBattlePoints > b.characterBattlePoints AND m.wasDefeated != False GROUP BY b.monsterID;";
    
    // Request for Losing Characters
    char requestLosingC[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT b.characterMemberID FROM Battles b, Characters c WHERE b.monsterBattlePoints > b.characterBattlePoints AND c.wasDefeated != True GROUP BY b.characterMemberID;";
    // Request for Winning Characters
    char requestWinningC[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT b.characterMemberID FROM Battles b, Characters c WHERE b.monsterBattlePoints < b.characterBattlePoints AND c.wasDefeated != False GROUP BY b.characterMemberID;";

    PGresult *res1 = PQexec(conn, requestLosingM);
    PGresult *res2 = PQexec(conn, requestWinningM);
    
    if(isC){
        PGresult *res1 = PQexec(conn, requestLosingC);
        PGresult *res2 = PQexec(conn, requestWinningC);
    }
    
    // Error checking
    if (PQresultStatus(res1) != PGRES_TUPLES_OK || PQresultStatus(res2) != PGRES_TUPLES_OK) {
        printf(PQerrorMessage(conn));      
        PQclear(res1);
        PQclear(res2);
        bad_exit(conn);
        return -1;
    }
    
    int rows1 = PQntuples(res1);
    if (rows1 == 0) { 
        printf("Update failed. requestLosingM returns no values\n");
        PQclear(res1);
        PQclear(res2);
        bad_exit(conn);
        return -1;
    }
    
    int rows2 = PQntuples(res2);
    if (rows2 == 0) { 
        printf("Update failed. requestLosingC returns no values\n");
        PQclear(res1);
        PQclear(res2);
        bad_exit(conn);
        return -1;
    }

    // Start updating
    // init as isM is 1
    char updateWinning[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Monsters SET wasDefeated = False WHERE monsterID = ";
    char updateLosing[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Monsters SET wasDefeated = True WHERE monsterID = ";
    int column = 0;
    
    if(isC){
        char updateWinning[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Characters SET wasDefeated = False WHERE memberID = ";
        char updateLosing[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Characters SET wasDefeated = True WHERE memberID = ";
    }

    // Update Winning
    for(int i = 0; i < rows2; i++){
        char *ID2 = PQgetvalue(res2, i, column);
        strcat(updateWinning, ID2);
        PGresult *resL2 = PQexec(conn, updateWinning);
        
        PQclear(resL2);
    }

    // Update Losing
    for(int i = 0; i < rows1; i++){
        char *ID1 = PQgetvalue(res1, i, column);
        strcat(updateLosing, ID1);
        PGresult *resW1 = PQexec(conn, updateLosing);
        
        PQclear(resW1);
    }

    PQclear(res1);
    PQclear(res2);
    return rows1 + rows2;
}

/* Function: increaseSomeThingCosts:
 * -------------------------------
 * Parameters:  connection, and an integer maxTotalIncrease, the maximum total increase that
 * it should make in the cost of some things.
 * Executes by invoking a Stored Function, increaseSomeThingCostsFunction, which
 * returns a negative value if there is an error, and otherwise returns the total
 * cost increase that was performed by the Stored Function.
 */

int increaseSomeThingCosts(PGconn *conn, int maxTotalIncrease)
{
    
}

int main(int argc, char **argv)
{
    PGconn *conn;
    int theResult;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./runAdventureApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        bad_exit(conn);
    }

    int result;
    
    /* Perform the calls to printNumberOfThingsInRoom listed in Section 6 of Lab4,
     * printing error message if there's an error.
     */
    printf("\n");
    printNumberOfThingsInRoom(conn, 11);
    printNumberOfThingsInRoom(conn, 1);
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to updateWasDefeated listed in Section 6 of Lab4,
     * and print messages as described.
     */
    printf("Updated %d Monsters\n", updateWasDefeated(conn, "M"));
    // printf("Updated %d Character\n", updateWasDefeated(conn, "C"));
    updateWasDefeated(conn, "D");
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to increaseSomeThingCosts listed in Section 6 of Lab4,
     * and print messages as described.
     */

    good_exit(conn);
    return 0;
}
