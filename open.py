#!/usr/bin/python


import commands
import sys


def runCommand(cmd, logPath=None):
	output = commands.getoutput(cmd)

	print output

	if logPath:
		file = open(logPath, "w")
		file.write(output)
		file.close()

	return

	subprocess.Popen(cmd, shell=True, stdout=subprocess.STDOUT, stderr=subprocess.STDOUT)
	return
	popen = subprocess.Popen([cmd], stdout=subprocess.PIPE, cwd=".")
	for line in iter(popen.stdout.readline, ""):
		print line,


def main():
	logPath="logs/clarity.log"

	if sys.platform == "cygwin":
		runCommand("build/debug/clarity.exe -f /home/uberska/clarity/one.txt -f /home/uberska/clarity/two.txt", logPath)
	else:
		runCommand("open build/debug/clarity.app -f /Users/scott/Desktop/clarity/one.txt -f /Users/scott/Desktop/clarity/two.txt", logPath)


main()

