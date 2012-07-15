# This is necessary to include the same source files in both executables
def create_objs(sourceFiles):
	ret = []
	for file in sourceFiles:
		ret.append(env.Object(file))
	return ret
		
# Build the VM program
env = Environment()
exe = 'vm'
src =  ['file.c']
env.Program(exe, create_objs(src + ['main.c']), compiler_options='-Wall -g -std=c99 -O0')

# Build the test program
testExe = 'test/vmTest'
testSrc = src + ['test/testMain.c','test/test_file.c']
env.Program(testExe, create_objs(testSrc), compiler_options='-Wall -g -std=c99 -O0')
