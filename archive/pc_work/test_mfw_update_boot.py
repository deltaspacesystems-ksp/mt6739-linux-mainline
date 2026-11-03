import contextlib
import io
from pathlib import Path
import struct
import sys
import unittest
from unittest.mock import patch
import mfw_update_boot as update

IMAGE = Path(r"D:\y5_linux_work\mfw_v51.img")

class ValidatorTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.valid = IMAGE.read_bytes()

    def test_built_image(self):
        info = update.validate_image(self.valid)
        self.assertEqual(info["image_bytes"], IMAGE.stat().st_size)
        self.assertLess(info["kernel_bytes"], 14*1024*1024)

    def test_reject_corrupt_payload(self):
        data = bytearray(self.valid)
        data[2500] ^= 1
        with self.assertRaisesRegex(ValueError, "SHA1"):
            update.validate_image(data)

    def test_reject_wrong_address_and_truncation(self):
        data = bytearray(self.valid)
        struct.pack_into("<I", data, 12, 0x40008000)
        with self.assertRaisesRegex(ValueError, "layout"):
            update.validate_image(data)
        with self.assertRaises(ValueError):
            update.validate_image(self.valid[:4096])

    def test_check_never_stages_or_writes(self):
        with patch.object(sys, "argv", ["updater", str(IMAGE)]), \
             patch.object(update, "inspect_phone", return_value="0"*64), \
             patch.object(update, "stage") as stage, \
             patch.object(update, "write_boot") as write, \
             contextlib.redirect_stdout(io.StringIO()):
            update.main()
            stage.assert_not_called()
            write.assert_not_called()

    def test_preflight_failure_never_writes(self):
        with patch.object(sys, "argv", ["updater", str(IMAGE), "--write"]), \
             patch.object(update, "inspect_phone", side_effect=RuntimeError("wrong target")), \
             patch.object(update, "stage") as stage, \
             patch.object(update, "write_boot") as write, \
             contextlib.redirect_stdout(io.StringIO()):
            with self.assertRaises(RuntimeError):
                update.main()
            stage.assert_not_called()
            write.assert_not_called()

if __name__ == "__main__":
    unittest.main()
