-- Constraint 1
ALTER TABLE Battles
ADD CONSTRAINT foreign_constraint1
FOREIGN KEY (monsterID) REFERENCES Monsters(monsterID)
    ON UPDATE CASCADE;

-- Constraint 2
ALTER TABLE Battles
ADD CONSTRAINT foreign_constraint2
FOREIGN KEY (characterMemberID, characterRole) REFERENCES Characters(memberID, role)
    ON DELETE CASCADE;

-- Constraint 3
ALTER TABLE Things
ADD CONSTRAINT foreign_constraint3
FOREIGN KEY (ownerMemberID, ownerRole) REFERENCES Characters(memberID, role)
    ON DELETE SET NULL
    ON UPDATE CASCADE;
