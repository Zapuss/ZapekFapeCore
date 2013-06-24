-- Item Throngus's Finger
DELETE FROM spell_proc_event WHERE entry = 92209;
DELETE FROM spell_proc_event WHERE entry = 92207;

INSERT INTO spell_proc_event (`entry`, `procEx`, `Cooldown`) VALUES (92209, 0x20, 60), (92207, 0x20, 60);
