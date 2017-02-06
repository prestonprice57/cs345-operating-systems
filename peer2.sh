#!/bin/bash
# Lab LinuxInfo
# Brother Jones, CS 345, Operating Systems
# Paul Mason
############################################################################
#
# Summary:
#    This program will test my knowledge on shell commands that will help me
#   to minyor the different aspects of the system.
#
############################################################################
#1
echo "What is the CPU type and model?"
#modelName=$(lscpu |grep 'Model name:' | awk '{print $3 $4 $5 $6 $7 $8 $9 $10 $11}')
modelName=$(lscpu |grep 'Model name:' | cut -c12-65)
echo ' ' $modelName $'\n'
#2
echo "How many processors are on this system?"
p=$(nproc)
echo ' ' $p $' processors\n'
#3
echo "What version of Linux kernel is being used?"
kernel1=$(more /proc/version | sed "s/([^)]*)//g" | cut -c1-40)
kernel2=$(more /proc/version | sed "s/([^)]*)//g" | cut -c45-80)
echo '  ' $kernel1 $kernel2 $'\n'
#4
echo "How long(in days, hours, and minutes) has it been since the system was last booted?"
boot=$(uptime | cut -c10-26)
echo ' ' $boot $'\n'
#5
echo "How much of the total CPU time available (across all processors) has been spent executing in User mode? System mode? Idle?"
userMode=$(head -1 /proc/stat | cut -c6-11)
systemMode=$(head -1 /proc/stat | cut -c17-24)
idle=$(head -1 /proc/stat | cut -c25-33)
hz=1000
echo '  USER:' $(bc<<<"scale=2;$userMode/$hz") $'seconds\n' ' SYSTEM:' $(bc<<<"scale=2;$systemMode/$hz") $'second\n' ' IDEL:' $(bc<<<"scale=2;$idle/$hz") $'seconds\n'
#6
echo "How much memory is on the machine?"
memory=$(grep "MemTotal:" /proc/meminfo)
echo '  ' $memory $'\n'
#7
echo "How much memory is currently available?"
aval=$(grep "MemFree:" /proc/meminfo)
echo '  ' $aval $'\n'
#8
echo "How many kBytes have been read from the hard disk (sda), and how many kBytes have been written to the hard disk, since the last system reboot?"
read=$(iostat -k | grep sda | awk '{print $5}')
write=$(iostat -k | grep sda | awk '{print $6}')
echo "  Read: "$read" kb"
echo "  Written: "$write" kb" $'\n'
#9
echo "How many processes have been created since the last reboot?"
processes=$(grep processes /proc/stat | awk '{print $2}')
echo '  Processes created:' $processes $'\n'
#10
echo " How many context switches have been performed since the last reboot?"
contextSwitches=$(grep ctxt /proc/stat | awk '{print $2}')
echo '  Context switches:' $contextSwitches $'\n'
#11                    
echo "What is the current load average for the last 1, 5, and 15 minutes?"
loadAverage=$(uptime | cut -c54-69)
echo '  Load average: ' $loadAverage $'\n'
