-- WrongWasDefeated

SELECT v.memberid AS theMemberID, v.role AS theRole, v.name AS theName, 
       v.fullBattlePoints AS theFullBattlePoints, COUNT(*) as numLosses
FROM FullBattlePointsView v, Battles b, Characters c
WHERE (v.memberID, v.role) = (c.memberID, c.role)
    AND c.wasDefeated = False
    AND v.name IS NOT NULL
    AND (v.memberID, v.role) = (b.characterMemberID, b.characterRole)
    AND b.characterBattlePoints < b.monsterBattlePoints
GROUP BY theMemberID, theRole, theName, theFullBattlePoints;

/* Output of the above query

 thememberid | therole | thename  | thefullbattlepoints | numlosses 
-------------+---------+----------+---------------------+-----------
         101 | knight  | Lancelot |                  56 |         4
         101 | mage    | Jack     |                 137 |         5
         111 | cleric  | Patrick  |                 215 |         3
*/

DELETE FROM Battles
WHERE (characterMemberID, characterRole, monsterID) = (111, 'cleric', 925)

DELETE FROM Battles
WHERE (characterMemberID, characterRole, monsterID) = (101, 'knight', 944)


/* Output of the above query (after the DELETE queries)

 thememberid | therole | thename  | thefullbattlepoints | numlosses 
-------------+---------+----------+---------------------+-----------
         101 | knight  | Lancelot |                  56 |         3
         101 | mage    | Jack     |                 137 |         5
         111 | cleric  | Patrick  |                 215 |         2
*/

/*
The result I received after running the DELETE queries was different from the first

I believe this is correct since the battles that involve (thememberid, role) of (101, knight) and (111, cleric)
were removed

This was reflected by the decrease in the numlosses counts

The tuple (101, mage, Jack, ...) did not experience any changes since any battles that involve it were intact
*/