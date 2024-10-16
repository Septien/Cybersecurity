
s = "e0 32 3a 0a 49 06 24 5c c2 d3 ac 62 91 95 e4 79\ne7 c8 37 6d 8d d5 4e a9 6c 56 f4 ea 65 7a ae 08\nba 78 25 2e 1c a6 b4 c6 e8 dd 74 1f 4b bd 8b 8a\n70 3e b5 66 48 03 f6 0e 61 35 57 b9 86 c1 1d 9e\ne1 f8 98 11 69 d9 8e 94 9b 1e 87 e9 ce 55 28 df\n8c a1 89 0d bf e6 42 68 41 99 2d 0f b0 54 bb 16"
s1 = "52 09 6a d5 30 36 a5 38 bf 40 a3 9e 81 f3 d7 fb\n7c e3 39 82 9b 2f ff 87 34 8e 43 44 c4 de e9 cb\n54 7b 94 32 a6 c2 23 3d ee 4c 95 0b 42 fa c3 4e\n08 2e a1 66 28 d9 24 b2 76 5b a2 49 6d 8b d1 25\n72 f8 f6 64 86 68 98 16 d4 a4 5c cc 5d 65 b6 92\n6c 70 48 50 fd ed b9 da 5e 15 46 57 a7 8d 9d 84\n90 d8 ab 00 8c bc d3 0a f7 e4 58 05 b8 b3 45 06\nd0 2c 1e 8f ca 3f 0f 02 c1 af bd 03 01 13 8a 6b\n3a 91 11 41 4f 67 dc ea 97 f2 cf ce f0 b4 e6 73\n96 ac 74 22 e7 ad 35 85 e2 f9 37 e8 1c 75 df 6e\n47 f1 1a 71 1d 29 c5 89 6f b7 62 0e aa 18 be 1b\nfc 56 3e 4b c6 d2 79 20 9a db c0 fe 78 cd 5a f4\n1f dd a8 33 88 07 c7 31 b1 12 10 59 27 80 ec 5f\n60 51 7f a9 19 b5 4a 0d 2d e5 7a 9f 93 c9 9c ef\na0 e0 3b 4d ae 2a f5 b0 c8 eb bb 3c 83 53 99 61\n17 2b 04 7e ba 77 d6 26 e1 69 14 63 55 21 0c 7d"
for l in s1.split('\n'):
    print('{', end='')
    for c in l.split(' '):
        print('0x'+c, end=', ')
    print('}')