-- Query 3 (Not work)
SELECT DISTINCT m.name as memberName, m.joinDate as memberJoinDate
FROM Members m
WHERE m.joinDate IS NOT NULL
AND (m.memberID) != ANY (SELECT t.ownerMemberID
                       FROM Things t)

-- SELECT * FROM Members WHERE joinDate IS NOT NULL

-- SELECT * FROM Characters

-- SELECT * FROM Things