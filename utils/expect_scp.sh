#!/usr/bin/expect -f
# Expect script to supply root/admin password for remote ssh server
# and execute command.
# This script needs four argument to(s) connect to remote server:
# user = Username for login to remote server
# password = Password of remote UNIX server.
# ipaddr = IP Addreess of remote UNIX server, no hostname
# scriptname = Path to remote script which will execute on remote server
# For example:
#  ./sshlogin.exp password 192.168.1.11 who
# ------------------------------------------------------------------------
# Copyright (c) 2011 mrTuborg
# This script is licensed under GNU GPL version 2.0 or above
# -------------------------------------------------------------------------
# This script is part of nixCraft shell script collection (NSSC)
# Visit http://bash.cyberciti.biz/ for more information.
# ----------------------------------------------------------------------
# set Variables
set password	[lrange $argv 0 0]
set file_local	[lrange $argv 1 1]
set file_remote	[lrange $argv 2 2]
set arg1	[lrange $argv 3 3]

set timeout -1
# now connect to remote UNIX box (ipaddr) with given script to execute
spawn scp $arg1 $file_local $file_remote
match_max 100000
# Look for passwod prompt
expect "*?assword:*"
# Send password aka $password
send -- "$password\r"
# send blank line (\r) to make sure we get back to gui
send -- "\r"
expect eof
