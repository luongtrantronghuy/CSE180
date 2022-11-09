-- Constraint 1
ALTER TABLE Things
ADD CONSTRAINT positiveCost
    CHECK (cost > 0);

-- Constraint 2
ALTER TABLE Monsters
ADD CONSTRAINT majorMonsters
    CHECK (battlePoints < 40 OR (battlePoints >= 40 AND (monsterKind = 'gi' OR monsterKind= 'ba' OR monsterKind IS NULL )));

-- Constraint 3
ALTER TABLE Members
ADD CONSTRAINT expirationCurrrent
    CHECK (expirationDate IS NOT NULL OR (expirationDate IS NULL AND isCurrent IS NULL));