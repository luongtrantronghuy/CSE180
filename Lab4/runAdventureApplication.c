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
        return -1;
    }

    // If no rows were returned, return with -1
    int rows = PQntuples(res);
    if (rows == 0) { 
        // printf("RoomID %s not found!\n", theRoomIDstr);
        PQclear(res);
        return -1;
    }

    // Get values from the return row (should only be one)
    char *desc = PQgetvalue(res, 0, 0);
    char *count = PQgetvalue(res, 0, 1);
    // printf("Room %s, %s, has %s in it.\n", theRoomIDstr, desc, count);
    
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
    if(strcmp(doCharactersOrMonsters, "M") == 0){
        // Request monsters that lose at least 1
        char requestL[MAXCONNECTIONSTRINGSIZE] = "SELECT monsterID FROM Battles WHERE characterBattlePoints > monsterBattlePoints GROUP BY monsterID";
        char requestTrueL[MAXCONNECTIONSTRINGSIZE] = "SELECT monsterID from Monsters WHERE wasDefeated = FALSE AND monsterID IN (";
        strcat(requestTrueL, requestL);
        strcat(requestTrueL, ")");
        // Request monsters that ONLY wins
        char requestW[MAXCONNECTIONSTRINGSIZE] = "SELECT monsterID FROM Battles WHERE characterBattlePoints < monsterBattlePoints AND monsterID NOT IN (SELECT monsterID FROM Battles WHERE characterBattlePoints > monsterBattlePoints GROUP BY monsterID) GROUP BY monsterID";
        char requestTrueW[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT monsterID FROM Monsters WHERE wasDefeated = TRUE AND monsterID IN (";
        strcat(requestTrueW, requestW);
        strcat(requestTrueW, ")");
        
        PGresult *resL = PQexec(conn, requestTrueL);
        PGresult *resW = PQexec(conn, requestTrueW);
        
        if (PQresultStatus(resL) != PGRES_TUPLES_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(resL);
            return -1;
        }

        if (PQresultStatus(resW) != PGRES_TUPLES_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(resW);
            return -1;
        }
        // If no rows were returned, return with -1
        int numL = PQntuples(resL);
        int numW = PQntuples(resW);

        if (numL == 0 && numW == 0) { 
            // printf("No Monsters To Update\n");
            PQclear(resL);
            return 0;
        }

        // printf("Found %d Losing and %d Winning Monsters\n", numL, numW);

        char updateWinning[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Monsters SET wasDefeated = FALSE WHERE monsterID IN (";
        char updateLosing[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Monsters SET wasDefeated = TRUE WHERE monsterID IN (";

        // Update Winning
        strcat(updateWinning, requestTrueW);
        strcat(updateWinning, ")");

        // Update Losing
        strcat(updateLosing, requestTrueL);
        strcat(updateLosing, ")");

        PGresult *result = PQexec(conn, updateWinning);
        PGresult *result1 = PQexec(conn, updateLosing);

        // Error checking
        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(result);
            return -1;
        }
        if (PQresultStatus(result1) != PGRES_COMMAND_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(result1);
            return -1;
        }

        PQclear(result);
        PQclear(result1);
        return numW + numL;
    
    }else if(strcmp(doCharactersOrMonsters, "C") == 0){
        // Request Characters that lose at least 1
        char requestL[MAXCONNECTIONSTRINGSIZE] = "SELECT characterMemberID, characterRole FROM Battles WHERE characterBattlePoints < monsterBattlePoints GROUP BY characterMemberID, characterRole";
        char requestTrueL[MAXCONNECTIONSTRINGSIZE] = "SELECT memberID, role from Characters WHERE wasDefeated = FALSE AND (memberID, role) IN (";
        strcat(requestTrueL, requestL);
        strcat(requestTrueL, ")");
        // Request Characters that ONLY wins
        char requestW[MAXCONNECTIONSTRINGSIZE] = "SELECT characterMemberID, characterRole FROM Battles WHERE characterBattlePoints > monsterBattlePoints AND (characterMemberID, characterRole) NOT IN (SELECT characterMemberID, characterRole FROM Battles WHERE characterBattlePoints < monsterBattlePoints GROUP BY characterMemberID, characterRole) GROUP BY characterMemberID, characterRole";
        char requestTrueW[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT memberID, role FROM Characters WHERE wasDefeated = TRUE AND (memberID, role) IN (";
        strcat(requestTrueW, requestW);
        strcat(requestTrueW, ")");

        // printf(requestTrueL);
        // printf("\n");
        // printf(requestTrueW);
        // printf("\n");
        
        PGresult *resL = PQexec(conn, requestTrueL);
        PGresult *resW = PQexec(conn, requestTrueW);
        
        if (PQresultStatus(resL) != PGRES_TUPLES_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(resL);
            return -1;
        }

        if (PQresultStatus(resW) != PGRES_TUPLES_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(resW);
            return -1;
        }
        // If no rows were returned, return with -1
        int numL = PQntuples(resL);
        int numW = PQntuples(resW);

        if (numL == 0 && numW == 0) { 
            // printf("No Characters To Update\n");
            PQclear(resL);
            return 0;
        }

        // printf("Found %d Losing and %d Winning Characters\n", numL, numW);

        char updateWinning[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Characters SET wasDefeated = FALSE WHERE (memberID, role) IN (";
        char updateLosing[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Characters SET wasDefeated = TRUE WHERE (memberID, role) IN (";

        // Update Winning
        strcat(updateWinning, requestTrueW);
        strcat(updateWinning, ")");

        // Update Losing
        strcat(updateLosing, requestTrueL);
        strcat(updateLosing, ")");

        PGresult *result = PQexec(conn, updateWinning);
        PGresult *result1 = PQexec(conn, updateLosing);

        // Error checking
        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(result);
            return -1;
        }
        if (PQresultStatus(result1) != PGRES_COMMAND_OK) {
            printf(PQerrorMessage(conn));      
            PQclear(result1);
            return -1;
        }

        PQclear(result);
        PQclear(result1);
        return numW + numL;
    }else{
        printf("Not M or C\n");
        return -1;
    }

    
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
    // Build sql statement to call function
    char callToFunc[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT increaseSomeThingCostsFunction(";
    char maxTotalIncreaseStr[100];
    sprintf(maxTotalIncreaseStr, "%d", maxTotalIncrease);
    strcat(callToFunc, maxTotalIncreaseStr);
    strcat(callToFunc, ")");

    PGresult *result = PQexec(conn, callToFunc);

    // If there's an error, exit with bad_exit
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        printf(PQerrorMessage(conn));      
        PQclear(result);
        return -1;
    }

    int totalIncreased = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);

    return totalIncreased;

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
    
    /* Perform the calls to printNumberOfThingsInRoom listed in Section 6 of Lab4,
     * printing error message if there's an error.
     */
    printf("\n");

    int roomID;
    int result;

    roomID = 1;
    result = printNumberOfThingsInRoom(conn, roomID);
    switch(result) {
        case 0:
            break;
        case -1:
            printf("No room exists whose id is %d\n", roomID);
            break;
        default:
            printf("Unknown Error (printNumberOfThingsInRoom with roomID %d)\n", roomID);
            bad_exit(conn);
            break;
    }
    
    roomID = 2;
    result = printNumberOfThingsInRoom(conn, roomID);
    switch(result) {
        case 0:
            break;
        case -1:
            printf("No room exists whose id is %d\n", roomID);
            break;
        default:
            printf("Unknown Error (printNumberOfThingsInRoom with roomID %d)\n", roomID);
            bad_exit(conn);
            break;
    }
    
    roomID = 3;
    result = printNumberOfThingsInRoom(conn, roomID);
    switch(result) {
        case 0:
            break;
        case -1:
            printf("No room exists whose id is %d\n", roomID);
            break;
        default:
            printf("Unknown Error (printNumberOfThingsInRoom with roomID %d)\n", roomID);
            bad_exit(conn);
            break;
    }
    
    roomID = 7;
    result = printNumberOfThingsInRoom(conn, roomID);
    switch(result) {
        case 0:
            break;
        case -1:
            printf("No room exists whose id is %d\n", roomID);
            break;
        default:
            printf("Unknown Error (printNumberOfThingsInRoom with roomID %d)\n", roomID);
            bad_exit(conn);
            break;
    }
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to updateWasDefeated listed in Section 6 of Lab4,
     * and print messages as described.
     */
    // printf("Updated %d Monsters\n", updateWasDefeated(conn, "M"));

    int result2;
    char *letter;

    letter = "C";
    result2 = updateWasDefeated(conn, letter);
    if(result2 >= 0){
        printf("%d wasDefeated values were fixed for %s\n", result2, letter);
    }else if (result2 == -1){
        printf("Illegal value for doCharactersOrMonsters %s\n", letter);
    }else {
        printf("Bad return value %d (updateWasDefeated with input %s)\n", result2, letter);
        bad_exit(conn);
    }
    
    // switch(result2 > 0) {
    //     case 1:
    //         printf("%d wasDefeated values were fixed for %s\n", result2, letter);
    //         break;
    //     case 0:
    //     switch(result2 < 0):
    //         case -1:
    //             printf("Illegal value for doCharactersOrMonsters %s\n", letter);
    //             break;
    //     default:
    //         printf("Bad return value %d (updateWasDefeated with input %s)\n", result2, letter);
    //         bad_exit(conn);
    //         break;
    // }

    letter = "M";
    result2 = updateWasDefeated(conn, letter);
    if(result2 >= 0){
        printf("%d wasDefeated values were fixed for %s\n", result2, letter);
    }else if (result2 == -1){
        printf("Illegal value for doCharactersOrMonsters %s\n", letter);
    }else {
        printf("Bad return value %d (updateWasDefeated with input %s)\n", result2, letter);
        bad_exit(conn);
    }

    letter = "C";
    result2 = updateWasDefeated(conn, letter);
    if(result2 >= 0){
        printf("%d wasDefeated values were fixed for %s\n", result2, letter);
    }else if (result2 == -1){
        printf("Illegal value for doCharactersOrMonsters %s\n", letter);
    }else {
        printf("Bad return value %d (updateWasDefeated with input %s)\n", result2, letter);
        bad_exit(conn);
    }
    
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to increaseSomeThingCosts listed in Section 6 of Lab4,
     * and print messages as described.
     */
    int maxTotalIncrease;
    int result3;

    maxTotalIncrease = 12;
    result3 = increaseSomeThingCosts(conn, maxTotalIncrease);
    if(result3 >= 0){
        printf("Total increase for maxTotalIncrease %d is %d\n", maxTotalIncrease, result3);
    }else {
        printf("Bad return value %d (increaseSomeThingCosts with input %s)\n", result3, maxTotalIncrease);
        bad_exit(conn);
    }

    maxTotalIncrease = 500;
    result3 = increaseSomeThingCosts(conn, maxTotalIncrease);
    if(result3 >= 0){
        printf("Total increase for maxTotalIncrease %d is %d\n", maxTotalIncrease, result3);
    }else {
        printf("Bad return value %d (increaseSomeThingCosts with input %s)\n", result3, maxTotalIncrease);
        bad_exit(conn);
    }

    maxTotalIncrease = 39;
    result3 = increaseSomeThingCosts(conn, maxTotalIncrease);
    if(result3 >= 0){
        printf("Total increase for maxTotalIncrease %d is %d\n", maxTotalIncrease, result3);
    }else {
        printf("Bad return value %d (increaseSomeThingCosts with input %s)\n", result3, maxTotalIncrease);
        bad_exit(conn);
    }

    maxTotalIncrease = 1;
    result3 = increaseSomeThingCosts(conn, maxTotalIncrease);
    if(result3 >= 0){
        printf("Total increase for maxTotalIncrease %d is %d\n", maxTotalIncrease, result3);
    }else {
        printf("Bad return value %d (increaseSomeThingCosts with input %s)\n", result3, maxTotalIncrease);
        bad_exit(conn);
    }


    good_exit(conn);
    return 0;
}
