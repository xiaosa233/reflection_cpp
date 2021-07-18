class googletest:
    def __init__(self, config):
        self.include_dirs = ['include']

        if config.system == config.System.LINUX:
            self.lib_dirs = ['lib',]
            self.libs = [
                'libgtest.a',
                'libgtest_main.a',
            ]
            self.system_libs = [
                'pthread',
            ]
        else:
            if config.configuration == config.Configuration.DEBUG:
                self.lib_dirs = ['lib/gtest/Debug']
                self.libs = ['*.lib']
            elif config.configuration == config.Configuration.RELEASE:
                self.lib_dirs = ['lib/gtest/Release']
                self.libs = ['*.lib']
