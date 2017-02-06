#!/bin/bash
# Lab LinuxInfo
# Brother Jones, CS 345, Operating Systems
# Nathan Hobley
############################################################################
#
# Summary:
#    Prints a bunch of information about the operating system.
#
#############################################################################
echo "What is the CPU type and model?"
echo -n "    "
grep "model name" /proc/cpuinfo | tail -1 | awk -F ": " '{print $2}'
echo
echo "How many processors on this system?"
echo -n "    "
echo -n $(grep "processor" /proc/cpuinfo | nl | tail -1 | cut -b6)
echo " processors"
echo
echo "What version of the Linux kernel is being used?"
echo -n "    "
grep "" /proc/version | sed -e 's/ (.*)//g'
echo
echo "How long has it been since the system was last booted?"
echo -n "    "
uptime | cut -b11-26
echo
echo "How much CPU execution time has been spent in user, system and idle modes?"
echo -n "    USER: "
echo -n $(echo "scale=2;" $(($(grep "cpu " /proc/stat | awk -F ' ' '{print $2}') + $(grep "cpu " /proc/stat | awk -F ' ' '{print $3}')))/100 | bc)
echo " seconds"
echo -n "    SYSTEM: "
echo -n $(echo "scale=2;" $(grep "cpu " /proc/stat | awk -F ' ' '{print $4}')/100 | bc)
echo " seconds"
echo -n "    IDLE: "
echo -n $(echo "scale=2;" $(grep "cpu " /proc/stat | awk -F ' ' '{print $5}')/100 | bc)
echo " seconds"
echo
echo "How much memory is on the machine?"
echo -n "    "
grep "MemTotal" /proc/meminfo
echo
echo "How much memory is currently available?"
echo -n "    "
grep "MemFree" /proc/meminfo
echo
echo "How many kBytes have been read and written to the disk since the last reboot?"
echo -n "    Read: "
echo -n $(iostat sda | grep sda | awk '{print $5}')
echo " kB"
echo -n "    Written: "
echo -n $(iostat sda | grep sda | awk '{print $6}')
echo " kB"
echo
echo "How many processes have been created since the last reboot?"
echo -n "    Processes created: "
grep "processes" /proc/stat | awk '{print $2}'
echo
echo "How many context switches have been performed since the last reboot?"
echo -n "    Context switches: "
grep "ctxt" /proc/stat | awk '{print $2}'
echo
echo "What is the current load average for the last 1, 5 and 15 minutes?"
echo -n "    L"
uptime | cut -b41-100
