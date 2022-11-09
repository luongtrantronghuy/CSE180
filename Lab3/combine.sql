BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;

-- Update Members from ModifyMembers if a tuple is in Members
UPDATE Members m
    SET memberID = mm.memberID, name = mm.name, address = mm.address, expirationDate = mm.expirationDate, isCurrent = TRUE
    FROM ModifyMembers mm
    WHERE m.memberID = mm.memberID;

-- Insert to Members from ModifyMembers if a tuple is not in Members
INSERT INTO Members(memberID, name, address, joinDate, expirationDate, isCurrent)
    SELECT mm.memberID, mm.name, mm.address, CURRENT_DATE, mm.expirationDate, NULL
    FROM ModifyMembers mm
    WHERE mm.memberID NOT IN (SELECT memberID FROM Members);

COMMIT;