#!/usr/bin/env python

import argparse
import socket
import threading
import signal
import time
from threading import Thread

questions = [
    "Does god exist?",
    "Why do we have to learn about git?",
    "Do you like coffee?",
    "Do you like chocolate",
    "How was your first day at altran?",
    "What's the answer to life, the universe and everything?",
    "What's your favorite food?",
]

g_sock = -1

def cleanup(signum, frame):
    print 'Asker stopped'
    g_event.set()
    g_sock.close()
    time.sleep(1)
    exit()

def init_sock(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((host, port))
    except socket.error as msg:
        print 'Connect failed. Error code : {} Message: {}'.format(str(msg[0]), msg[1])
        exit()
    return s

def run_cmd(sock, cmd):
    print 'Send cmd: [{}]'.format(cmd)
    sock.sendall(cmd, socket.MSG_DONTWAIT)
    data = sock.recv(1024)
    print ' <== Received cmd: {}'.format(data)
    return 0

def ask(idx, host, port):
    print "Asking question {} on host {}:{}".format(idx, host, port)
    sock = init_sock(host, port)
    socket.setdefaulttimeout(5)
    err = run_cmd(sock, questions[idx])
    if err != 0:
        print "Command failed, stopping execution!"
        return -1
    return 0

parser = argparse.ArgumentParser(description='W')
parser.add_argument("host", help='target host')
parser.add_argument("port", type=int, help='target port')
parser.add_argument("question", type=int, help='Question you would like to ask')
args = parser.parse_args()

print 'To terminate, press CTRL-Z'

signal.signal(signal.SIGHUP, cleanup)
signal.signal(signal.SIGTERM, cleanup)
signal.signal(signal.SIGTSTP, cleanup)

if args.question >= len(questions):
    print "There are only " + str(len(questions)) + " questions"
    exit(42)

if args.question >= 0:
    ret = ask(args.question, args.host, args.port)
    exit(ret)


