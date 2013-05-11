#!/usr/bin/python2.7
import os
import sys
import subprocess
import multiprocessing

#############################################################
def keepItems(allFiles, extension):
  vmFiles = []
  for fileName in allFiles:
    if (fileName[-3:] == extension):
      vmFiles.append(fileName)
  return vmFiles

#############################################################
# Get a list of all vm files
#############################################################
def fileList():
  # Either assume vm files are in current directory, or let user specify directory
#  directory = os.getcwd()
#  if (len(sys.argv) == 2):
#    directory = sys.argv[1]

  allFiles = os.listdir(os.getcwd())
  vmFiles = keepItems(allFiles, '.vm')
  return vmFiles

#############################################################
# Create a list of commands for the pool of workers to execute.
# One command for each vm file to translate.
#############################################################
def buildCommandList(vmFiles):
  vmApp = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'vm')
  #vmApp = os.path.join(os.path.abspath(os.getcwd()), 'vm')
  print vmApp
  commandList = [vmApp + ' ' + vmFile for vmFile in vmFiles]
  print commandList
  return commandList

#############################################################
def translateFile(command):
  #proc = subprocess.call('./vm ' + vmFile)
  print command
  proc = subprocess.call(command)
  return proc

#############################################################
# Translate each vm file into a separate asm file
#############################################################
def translate(vmFiles):
  pool = multiprocessing.Pool(None)
  commandList = buildCommandList(vmFiles)
  print pool.map(translateFile, commandList)
  #print pool.map(translateFile, ['ls'] * 4)
  return 

#############################################################
def main():
  vmFiles = fileList()
  print 'fileList: ' + str(vmFiles)
  asmFiles = translate(vmFiles)
  #combine(asmFiles)
  return

#############################################################
if __name__ == "__main__":
  main()

# get the directory containing the vm files
# for each vm file in the directory
  # run vm on it to get an asm file
# Initialize the stack pointer to 256 at the beginning of the final asm file
# concatenate the asm files into a single big asm file
# delete the other asm filestarts
