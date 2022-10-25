-- Query 1
SELECT r.roomID as roomID, r.roomDescription as initialRoomDescription, t.thingID, t.cost
FROM Rooms r, Things t
WHERE t.cost < 12
    AND t.thingKind = 'sc'
    AND r.roomID = t.initialRoomID
    AND r.roomDescription LIKE '_w%'
ORDER BY initialRoomDescription, cost DESC