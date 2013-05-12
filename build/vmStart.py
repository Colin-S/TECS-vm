#!/usr/bin/python2.7
import os
import sys
import subprocess
import multiprocessing
import shutil

#############################################################
def keepItems(allFiles, extension):
  keptFiles = []
  for fileName in allFiles:
    if (fileName.lower().endswith(extension)):
      keptFiles.append(fileName)
  return keptFiles

#############################################################
# Get a list of all files in the working directory,
#  with extension
#############################################################
def fileList(extension):
  # Assumes vm files are in current working directory
  allFiles = os.listdir(os.getcwd())
  files = keepItems(allFiles, extension)
  return files

#############################################################
# Create a list of commands for the pool of workers to execute.
# One command for each vm file to translate.
#############################################################
def buildCommandList(vmFiles):
  vmApp = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'vm')
  commandList = []
  for vmFile in vmFiles:
    commandList.append([vmApp, vmFile])
  print commandList
  return commandList

#############################################################
def translateFile(command):
  print 'translateFile ' + str(command)
  proc = subprocess.call(command)
  return proc

#############################################################
# Translate each vm file into a separate asm file
#############################################################
def translate(vmFiles):
  pool = multiprocessing.Pool(None)
  commandList = buildCommandList(vmFiles)
  returnVals = pool.map(translateFile, commandList)
  if any(val != 0 for val in returnVals):
    print '[ERROR] vm process failed'
  return 

#############################################################
def combine(asmFiles):
  fullAsmFile = open(os.path.basename(os.getcwd()) + '.asm', 'w')

  # Add stack pointer initialization

  # Start by calling Sys.init function
  sysInit = (
    "// Init SP\n@256\nD=A\n@SP\nM=D\n"                 # Init stack pointer
    "@LCL\nM=-1\n"                                      # Write -1, to detect errors
    "@ARG\nM=-1\n"                                      # Write -1, to detect errors
    "@THIS\nM=-1\n"                                     # Write -1, to detect errors
    "@THAT\nM=-1\n"                                     # Write -1, to detect errors
    "// Call Sys.init 0\n"                          
    "@start.sys.init\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n" # push return address
    "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"            # push LCL
    "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"            # push ARG
    "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"           # push THIS
    "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"           # push THAT
    "@5\nD=A\n@0\nD=D+A\n@SP\nD=M-D\n@ARG\nM=D\n"       # ARG = SP-nArgs-5
    "@SP\nD=M\n@LCL\nM=D\n"                             # LCL = SP
    "@sys.init\n0;JMP\n"                                # goto function
    "(start.sys.init)\n"                                # return address
    "(halt_loop)\n@halt_loop\n0;JMP\n"                  # Infinite loop if program returns
    )
  fullAsmFile.write(sysInit)

  # Then add the contents of the asm files
  for asmFile in asmFiles:
    shutil.copyfileobj(open(asmFile, 'r'), fullAsmFile)

  fullAsmFile.close()
  return 

#############################################################
def deleteFiles(asmFiles):
  for asmFile in asmFiles:
    os.remove(asmFile)

#############################################################
def main():
  deleteFiles(fileList('.asm'))
  vmFiles = fileList('.vm')
  #print 'vmFiles: ' + str(vmFiles)
  translate(vmFiles)
  asmFiles = fileList('.asm')
  #print 'asmFiles: ' + str(asmFiles)
  combine(asmFiles)
  deleteFiles(asmFiles)
  return

#############################################################
if __name__ == "__main__":
  main()
