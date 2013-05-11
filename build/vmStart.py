#!/usr/bin/python2.7
import os
import sys
import subprocess
import multiprocessing

#############################################################
def keepItems(allFiles, extension, directory):
  vmFiles = []
  for fileName in allFiles:
    if (fileName[-3:] == extension):
      vmFiles.append(os.path.join(directory, fileName))
  return vmFiles

#############################################################
# Get a list of all vm files
#############################################################
def fileList():
  # Either assume vm files are in current directory, or let user specify directory
  directory = '.'
  if (len(sys.argv) == 2):
    directory = sys.argv[1]

  # Get the files in the specified directory
  allFiles = os.listdir(directory)

  # Filter out non-vm files
  vmFiles = keepItems(allFiles, '.vm', directory)
  return vmFiles

#############################################################
def buildCommandList(vmFiles):
  commandList = ['./vm ' + vmFile for vmFile in vmFiles]
  print commandList
  return commandList

#############################################################
def translateFile(vmFile):
  #proc = subprocess.call('./vm ' + vmFile)
  proc = subprocess.call(vmFile)
  return proc

#############################################################
# Translate each vm file into a separate asm file
#############################################################
def translate(vmFiles):
  pool = multiprocessing.Pool(None)
  print 'fileList: ' + str(vmFiles)
  commandList = buildCommandList(vmFiles)
  #print pool.map(translateFile, vmFiles)
  print pool.map(translateFile, ['ls'] * 4)
  return 

#############################################################
def main():
  vmFiles = fileList()
  #print 'fileList: ' + str(vmFiles)
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
