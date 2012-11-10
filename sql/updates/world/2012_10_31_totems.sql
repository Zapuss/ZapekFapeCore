-- Totemic Wrath
DELETE FROM `spell_proc_event` WHERE `entry` IN (77746);
INSERT INTO `spell_proc_event` VALUES (77746, 0x00, 0x0B, 0x02001010, 0x00800000, 0x00000000, 0x00004400, 0x00000000, 0, 100, 0);

-- Glyph of Healing Stream Totem
DELETE FROM `spell_proc_event` WHERE `entry` IN (55456);
INSERT INTO `spell_proc_event` VALUES (55456, 0x08, 0x0B, 0x00002000, 0x00000000, 0x00000000, 0x00004000, 0x00000000, 0, 100, 0);

-- (51483) Earth's Grasp (Rank 1)
DELETE FROM `spell_proc_event` WHERE `entry` IN (51483);
INSERT INTO `spell_proc_event` VALUES (51483, 0x01, 0x0B, 0x00000000, 0x00000000, 0x00000020, 0x00004000, 0x00000000, 0, 50, 0);

-- (51485) Earth's Grasp (Rank 1)
DELETE FROM `spell_proc_event` WHERE `entry` IN (51485);
INSERT INTO `spell_proc_event` VALUES (51485, 0x01, 0x0B, 0x00000000, 0x00000000, 0x00000020, 0x00004000, 0x00000000, 0, 100, 0);





