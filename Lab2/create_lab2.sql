
-- CSE 180 Fall 2022 Lab1 Solution

-- Following two lines are not needed in your solution.
DROP SCHEMA Lab2 CASCADE;
CREATE SCHEMA Lab2;
-- ALTER ROLE htluong SET SEARCH_PATH to Lab2;

-- The comments that appear below are not needed in your solution.

-- INT is equivalent to INTEGER.
-- DECIMAL is equivalent to NUMERIC.
-- CHAR is equivalent to CHARACTER.
-- Specific spacing doesn't matter.

-- Members(memberID, name, address, joinDate, expirationDate, isCurrent)  
CREATE TABLE Members (
    memberID INT PRIMARY KEY,
    name VARCHAR(50),
    address VARCHAR(50),
    joinDate DATE,
    expirationDate Date,
    isCurrent BOOLEAN
);

-- Primary Key memberID could appear as a schema element, instead of next to attribute.

-- Rooms(roomID, roomDescription, northNext, eastNext, southNext, westNext)
CREATE TABLE Rooms(
    roomID INT PRIMARY KEY,
    roomDescription VARCHAR(30),
    northNext INT REFERENCES Rooms(roomID),
    eastNext INT REFERENCES Rooms(roomID),
    southNext INT REFERENCES Rooms(roomID),
    westNext INT REFERENCES Rooms(roomID)
);

-- Primary Key roomID could appear as a schema element, instead of next to attribute.


-- Roles(role, battlePoints, InitialMoney)
CREATE TABLE Roles (
    role VARCHAR(6) PRIMARY KEY,
    battlePoints INT,
    initialMoney NUMERIC(5,2)
);

-- Primary Key role could appear as a schema element, instead of next to attribute.


-- Characters(memberID, role, name, roomID, currentMoney, wasDefeated)
CREATE TABLE Characters (
    memberID INT,
    role VARCHAR(6) ,
    name VARCHAR(50),
    roomID INT,
    currentMoney NUMERIC(5,2),
    wasDefeated BOOLEAN,
    PRIMARY KEY (memberID, role),
    FOREIGN KEY (memberID) REFERENCES Members,
    FOREIGN KEY (role) REFERENCES Roles,
    FOREIGN KEY (roomID) REFERENCES Rooms
);

-- Foreign Key specifications don't have to mention attribute names after REFERENCES when the
--   attribute names are identical in both the referencing and the referenced relations.
-- Foreign Keys involving a single attribute may be specified next to that attribute, rather than
--   as a schema element.


-- Things(thingID, thingKind, initialRoomID, ownerMemberID, ownerRole, cost, extraBattlePoints)
CREATE TABLE Things (
    thingID INT PRIMARY KEY,
    thingKind CHAR(2),
    initialRoomID INT,
    ownerMemberID INT,
    ownerRole VARCHAR(6),
    cost NUMERIC(4,2),
    extraBattlePoints INT,
    FOREIGN KEY (ownerMemberID, ownerRole) REFERENCES Characters(memberID, role),
    FOREIGN KEY (initialRoomID) REFERENCES Rooms(roomID)
);

-- Foreign Key specifications don't have to mention attribute names after REFERENCES when the
--   attribute names are identical in both the referencing and the referenced relations.
-- Foreign Keys involving a single attribute may be specified next to that attribute, rather than
--   as a schema element, as in:   ownerMemberID INT REFERENCES Members(MemberID)


-- Monsters(monsterID, monsterKind, name, battlePoints, roomID, wasDefeated)
CREATE TABLE Monsters (
    monsterID INT PRIMARY KEY,
    monsterKind CHAR(2),
    name VARCHAR(50),
    battlePoints INT,
    roomID INT REFERENCES Rooms,
    wasDefeated BOOLEAN
);

-- Primary Key role could appear as a schema element, instead of next to attribute.


-- Battles(characterMemberID, characterRole, characterBattlePoints, monsterID, monsterBattlePoints)
CREATE TABLE Battles(
    characterMemberID INT,
    characterRole VARCHAR(6),
    characterBattlePoints INT,
    monsterID INT,
    monsterBattlePoints INT,
    PRIMARY KEY (characterMemberID, characterRole, monsterID),
    FOREIGN KEY (characterMemberID, characterRole) REFERENCES Characters(memberID, role),
    FOREIGN KEY (monsterID) REFERENCES Monsters
);

-- Foreign Key specifications don't have to mention attribute names after REFERENCES when the
--   attribute names are identical in both the referencing and the referenced relations.
-- Foreign Keys involving a single attribute may be specified next to that attribute, rather than
--   as a schema element.