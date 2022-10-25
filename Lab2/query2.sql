-- Query 2
SELECT m.name as monsterName, m.monsterID
FROM Monsters m
WHERE m.wasDefeated = false 
AND m.monsterID IN (SELECT monsterID
                       FROM Battles a
                       WHERE a.monsterBattlePoints < a.characterBattlePoints)

-- SELECT DISTINCT m.name as monsterName, m.monsterID
-- FROM Monsters m
-- WHERE m.wasDefeated = false 

-- SELECT a.monsterID, a.monsterBattlePoints, a.characterBattlePoints
--     FROM Battles a
--     WHERE a.monsterBattlePoints < a.characterBattlePoints