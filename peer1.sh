#!/bin/bash

# Lab LinuxInfo
# Brother Jones, CS 345, Operating Systems
# Paden Jones

############################################################################
#
# Summary:
#    This shell script pulls various statistics about the current machine
#
############################################################################

echo "What is the CPU type and model?"
grep -m 1 "model name" /proc/cpuinfo | sed -e 's/^.*: /    /'

echo -e "\nHow many processors on this system?"
grep "processor" /proc/cpuinfo | wc -l | sed -e 's/^/    /' | sed -e 's/$/ processors/'

echo -e "\nWhat version of the Linux kernel is being used?"
cat /proc/version | sed 's/^/    /' | sed 's/(.*) //'

echo -e "\nHow long has it been since the system was last booted?"
uptime | awk '{print $2" "$3" "$4" "$5}' | sed 's/\(.*\),/\1 /' | sed 's/^/    /'

echo -e "\nHow much CPU execution time has been spent in user, system and idle modes?"
head -1 /proc/stat | awk '{tmp = ($2 + $3)/100 ; printf"%0.2f\n", tmp }' | sed 's/^/    USER: /' | sed 's/$/ seconds/'
head -1 /proc/stat | awk '{tmp = $4/100 ; printf"%0.2f\n", tmp }' | sed 's/^/    SYSTEM: /' | sed 's/$/ seconds/'
head -1 /proc/stat | awk '{tmp = $5/100 ; printf"%0.2f\n", tmp }' | sed 's/^/    IDLE: /' | sed 's/$/ seconds/'

echo -e "\nHow much memory is on the machine?"
grep "MemTotal" /proc/meminfo | sed -e 's/^/    /'

echo -e "\nHow much memory is currently available?"
grep "MemFree" /proc/meminfo | sed -e 's/^/    /'

echo -e "\nHow many kBytes have been read and written to the disk since the last reboot?"
iostat | grep sda | awk '{tmp = $5; printf"    Read: %d kB\n", tmp }'
iostat | grep sda | awk '{tmp = $6; printf"    Written: %d kB\n", tmp }'

echo -e "\nHow many processes have been created since the last reboot?"
grep "processes" /proc/stat | awk '{tmp = $2; printf"    Processes created: %d\n", tmp}'

echo -e "\nHow many context switches have been performed since the last reboot?"
grep "ctxt" /proc/stat | awk '{tmp = $2; printf"    Context switches: %d\n", tmp}'

echo -e "\nWhat is the current load average for the last 1, 5 and 15 minutes?"
cat /proc/loadavg | awk '{printf"    Load average: %0.2f, %0.2f, %0.2f\n", $1, $2, $3}'
