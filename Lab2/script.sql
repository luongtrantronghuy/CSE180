-- SELECT * FROM Members WHERE joinDate IS NOT NULL

SELECT t.ownerMemberID FROM Things t

-- SELECT DISTINCT * 
-- FROM Things t, Members m
-- WHERE m.memberID != ANY (
--     SELECT t2.ownerMemberID FROM Things t2
-- )
-- AND m.joinDate IS NOT NULL

-- Query 1
-- SELECT t.initialRoomID as roomID, r.roomDescription as initialRoomDescription, t.thingID, t.cost
-- FROM Rooms r, Things t
-- WHERE roomID = ANY (SELECT t.initialRoomID 
--                     FROM Things 
--                     WHERE SUBSTR(r.roomDescription, 2, 1) = 'w') 
--                 AND cost < 12
--                 AND t.thingKind = 'sc'
-- ORDER BY initialRoomDescription, cost DESC

-- Query 2
-- SELECT DISTINCT m.name as monsterName
-- FROM Monsters m, Battles b
-- WHERE m.wasDefeated = false 
-- AND m.monsterID = ANY (SELECT monsterID
--                        FROM Battles a
--                        WHERE a.monsterBattlePoints < a.characterBattlePoints)

-- Query 3 (Not work)
-- SELECT DISTINCT m.name as memberName, m.joinDate as memberJoinDate
-- FROM Members m
-- WHERE m.joinDate IS NOT NULL
-- AND (m.memberID != ALL (SELECT t.ownerMemberID
--                        FROM Things t)
-- OR m.memberID != ALL (SELECT c.memberID
--                       FROM Characters c))

-- Query 4
-- SELECT m.name as theMonster, c.name as theCharacter, r.role as theRole
-- FROM Monsters m, Characters c, Members mem, Roles r
-- WHERE 