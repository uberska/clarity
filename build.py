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
	qmakeCmd = ""
	if sys.platform == "cygwin":
		qmakeCmd = "qmake-qt4"
	else:
		qmakeCmd = "qmake -spec macx-g++"
	runCommand("cd build; %s" % qmakeCmd, "logs/build.qmake.log")

	runCommand("cd build; make", "logs/build.make.log")


main()

