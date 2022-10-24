-- Query 1
SELECT t.initialRoomID as roomID, r.roomDescription as initialRoomDescription, t.thingID, t.cost
FROM Rooms r, Things t
WHERE roomID = ANY (SELECT t.initialRoomID 
                    FROM Things 
                    WHERE SUBSTR(r.roomDescription, 2, 1) = 'w') 
                AND cost < 12
                AND t.thingKind = 'sc'
ORDER BY initialRoomDescription, cost DESC