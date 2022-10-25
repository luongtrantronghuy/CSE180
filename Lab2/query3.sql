-- Query 3
SELECT m.name as memberName, m.joinDate as memberJoinDate
FROM Members m
WHERE m.joinDate IS NOT NULL
AND ((m.memberID) NOT IN (SELECT t.ownerMemberID FROM Things t WHERE t.ownerMemberID IS NOT NULL)
OR (m.memberID) NOT IN (SELECT c.memberID FROM Characters c WHERE c.memberID IS NOT NULL))

-- SELECT c.memberID FROM Characters c WHERE c.memberID IS NOT NULL

-- SELECT t.ownerMemberID FROM Things t WHERE t.ownerMemberID IS NOT NULL
