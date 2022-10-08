CREATE TABLE Members (
    memberID    INT,
    name        CHAR(50),
    address     CHAR(50),
    joinDate    DATE,
    expirationDate  DATE,
    isCurrent   BOOLEAN,
    PRIMARY KEY (memberID)
);

CREATE TABLE Rooms (
    roomID      INT,
    roomDescription CHAR(30),
    northNext   INT,
    eastNext    INT,
    southNext   INT,
    westNext    INT,
    PRIMARY KEY (roomID)
);

CREATE TABLE Roles (
    role        CHAR(6),
    battlePoints    INT,
    initialMoney    DECIMAL(5, 2),
    PRIMARY KEY (role)
);

CREATE TABLE Characters (
    memberID    INT,
    role        CHAR(6),
    name        CHAR(50),
    roomID      INT,
    currentMoney    DECIMAL(5, 2),
    wasDefeated     BOOLEAN,
    PRIMARY KEY (memberID, role),
    FOREIGN KEY (memberID) REFERENCES Members,
    FOREIGN KEY (role) REFERENCES Roles,
    FOREIGN KEY (roomID) REFERENCES Rooms
);

CREATE TABLE Things (
    thingID     INT,
    thingKind   CHAR(2),
    initialRoomID   INT,
    ownerMemberID   INT,
    ownerRole       CHAR(6),
    cost            NUMERIC (4, 2),
    extraBattlePoints   INT,
    PRIMARY KEY (thingID),
    FOREIGN KEY (initialRoomID) REFERENCES Rooms,
    FOREIGN KEY (ownerMemberID, ownerRole) REFERENCES Characters
);

CREATE TABLE Monsters (
    monsterID       INT,
    monsterKind     CHAR(2),
    name            CHAR(50),
    battlePoints    INT,
    roomID          INT,
    wasDefeated     BOOLEAN,
    PRIMARY KEY (monsterID),
    FOREIGN KEY (monsterID) REFERENCES Monsters
);

CREATE TABLE Battles (
    characterMemberID   INT,
    characterRole       CHAR(6),
    characterBattlePoints   INT,
    monsterID           INT,
    monsterBattlePoints INT,
    PRIMARY KEY (characterMemberID, characterRole, monsterID),
    FOREIGN KEY (characterMemberID, characterRole) REFERENCES Characters,
    FOREIGN KEY (monsterID) REFERENCES Monsters
);
