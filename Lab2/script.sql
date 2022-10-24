-- SELECT * FROM Rooms WHERE SUBSTR(roomDescription, 2, 1) = 'w'

-- SELECT * FROM Things WHERE cost < 12

-- Query 1
SELECT t.initialRoomID as roomID, r.roomDescription as initialRoomDescription, t.thingID, t.cost
FROM Rooms r, Things t
WHERE roomID = ANY (SELECT t.initialRoomID 
                    FROM Things 
                    WHERE SUBSTR(r.roomDescription, 2, 1) = 'w') 
                AND cost < 12
ORDER BY initialRoomDescription, cost DESC