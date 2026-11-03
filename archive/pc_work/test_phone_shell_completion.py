"""Regression: telnet echo and split TCP reads must not finish commands early."""
import unittest
from phone_shell import completed_status

class CompletionTests(unittest.TestCase):
    def test_echo_is_not_result(self):
        self.assertIsNone(completed_status(b'~ # echo __Y5END__ $?\r\n'))
        self.assertIsNone(completed_status(b'echo __Y5END__ $?\n~ # echo __Y5END__ $?\n'))

    def test_fragmented_status_waits_for_newline(self):
        data=b'~ # echo __Y5END__ $?\r\n'
        for fragment in (b'__Y5',b'END__ ',b'1',b'2',b'7',b'\r'):
            data+=fragment
            self.assertIsNone(completed_status(data))
        self.assertEqual(completed_status(data+b'\n~ # '),'127')

    def test_complete_success_and_failure(self):
        self.assertEqual(completed_status(b'output\r\n__Y5END__ 0\r\n~ # '),'0')
        self.assertEqual(completed_status(b'__Y5END__ 255\n'),'255')
        self.assertIsNone(completed_status(b'__Y5END__ nope\n'))
        self.assertIsNone(completed_status(b'__Y5END__ 999\n'))

if __name__=='__main__':unittest.main()
