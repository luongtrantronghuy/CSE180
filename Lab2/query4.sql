-- Query 4
SELECT m.name as theMonster, c.name as theCharacter, b.characterRole as theRole
FROM Monsters m, Characters c, Members mem, Roles r, Battles b
WHERE b.characterRole != 'knight'
AND m.monsterKind = 'ba'
AND (b.characterMemberID, b.characterRole) = (mem.memberID, c.role)
AND b.monsterID = m.monsterID
AND c.memberID = mem.memberID
AND (mem.expirationDate >= '2022-10-12')
AND r.role = b.characterRole
AND c.currentMoney > r.initialMoney
