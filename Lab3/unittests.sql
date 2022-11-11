-- Test for foreign constraints
-- monsterID 1920 not in table Monsters
-- INSERT INTO Battles
-- VALUES (102, 'cleric', 67, 1920, 7)

-- characterMemberID and characterRole (1920, cleric) not in table Characters
-- INSERT INTO Battles
-- VALUES (1920, 'cleric', 67, 988, 7)

-- (ownerMemberID, ownerRole) (1920, cleric) not in table Characters
-- INSERT INTO Things
-- VALUES (1111, 'sw', 3, 1920, 'cleric', 20.25, 18)

------------------------------------------------------

-- Test for general constraints
-- general 1: cost is postive so it's allowed
-- UPDATE Things
--     SET cost = 1
--     WHERE thingID = 6007

-- general 1: cost in Things must be positive
-- UPDATE Things
--     SET cost = 0
--     WHERE thingID = 6007

-- general 2: monsterKind can be NULL if battlePoint is >= 40
-- UPDATE Monsters
--     SET monsterKind = NULL
--     WHERE battlePoints >= 40

-- general 2: monsterKind can't be 'ga' if battlePoint is >= 40
-- UPDATE Monsters
--     SET monsterKind = 'ga'
--     WHERE battlePoints >= 40

-- general 3: isCurrent is allow to be anything when expirationDate isn't NULL
--            including NULL
-- UPDATE Members
--     SET isCurrent = NULL
--     WHERE expirationDate IS NOT NULL

-- general 3: isCurrent can't be NULL if expirationDate is NULL
-- UPDATE Members
--     SET isCurrent = TRUE
--     WHERE expirationDate IS NULL
