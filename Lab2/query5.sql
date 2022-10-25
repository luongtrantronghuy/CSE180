-- Query 5
SELECT r1.roomID as eastRoomID, r1.roomDescription as eastRoomDescription,
        r2.roomID as westRoomID, r2.roomDescription as westRoomDescription
FROM Rooms r1, Rooms r2
WHERE r1.roomID = r2.eastNext
AND r2.roomID = r1.westNext