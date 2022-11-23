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
        // printf(PQerrorMessage(conn));      
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
    if(!strcmp(doCharactersOrMonsters, "M") == 0 && !strcmp(doCharactersOrMonsters, "C") == 0) {
        printf("doCharactersOrMonsters is %s instead of M or C\n", doCharactersOrMonsters);
        return -1;
    }
    return 0;
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
    updateWasDefeated(conn, "A");
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to increaseSomeThingCosts listed in Section 6 of Lab4,
     * and print messages as described.
     */

    good_exit(conn);
    return 0;
}
