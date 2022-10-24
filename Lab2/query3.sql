-- Query 3
SELECT DISTINCT m.name as memberName, m.joinDate as memberJoinDate
FROM Members m, Characters c
WHERE m.joinDate IS NOT NULL
AND (m.memberID) NOT IN (SELECT t.ownerMemberID FROM Things t WHERE t.ownerMemberID IS NOT NULL)
