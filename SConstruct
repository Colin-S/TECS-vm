# This function is necessary to include the same source files in both executables
def create_objs(sourceFiles):
	ret = []
	for file in sourceFiles:
		ret.append(env.Object(file))
	return ret
		
env = Environment(CCFLAGS = '-g -Wall -O0 -std=c99', CC = 'gcc')

# Build the VM program
exe = 'vm'
src =  ['file.c', 'parser.c']
env.Program(exe, create_objs(src + ['main.c']))

# Build the test program
testExe = 'test/vmTest'
testSrc = src + ['test/testMain.c','test/test_file.c', 'test/test_parser.c']
env.Program(testExe, create_objs(testSrc))
