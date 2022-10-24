-- SELECT * FROM Monsters m WHERE m.wasDefeated = false

-- SELECT * FROM Battles

-- Query 1
-- SELECT t.initialRoomID as roomID, r.roomDescription as initialRoomDescription, t.thingID, t.cost
-- FROM Rooms r, Things t
-- WHERE roomID = ANY (SELECT t.initialRoomID 
--                     FROM Things 
--                     WHERE SUBSTR(r.roomDescription, 2, 1) = 'w') 
--                 AND cost < 12
-- ORDER BY initialRoomDescription, cost DESC

-- Query 2
SELECT DISTINCT m.name as monsterName
FROM Monsters m, Battles b
WHERE m.wasDefeated = false 
AND m.monsterID = ANY (SELECT monsterID
                       FROM Battles a
                       WHERE a.monsterBattlePoints < a.characterBattlePoints)