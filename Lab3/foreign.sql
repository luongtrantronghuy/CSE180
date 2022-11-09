-- Constraint 1
ALTER TABLE Battles
ADD CONSTRAINT constraint1
FOREIGN KEY Battles(monsterID) REFERENCES Monsters(monsterID)
    ON UPDATE CASCADE;

-- Constraint 2
ALTER TABLE Battles
ADD CONSTRAINT constraint2
FOREIGN KEY Battles(characterMemberID, characterRole) REFERENCES Characters(memberID, role)
    ON DELETE CASCADE;

-- Constraint 3
ALTER TABLE Things
ADD CONSTRAINT constraint3
FOREIGN KEY Things(ownerMemberID, ownerRole) REFERENCES Characters(memberID, role)
    ON DELETE SET NULL
    ON UPDATE CASCADE;
