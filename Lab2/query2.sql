-- Query 2
SELECT DISTINCT m.name as monsterName
FROM Monsters m, Battles b
WHERE m.wasDefeated = false 
AND m.monsterID = ANY (SELECT monsterID
                       FROM Battles a
                       WHERE a.monsterBattlePoints < a.characterBattlePoints)