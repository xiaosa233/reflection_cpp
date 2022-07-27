class reflection:
    def __init__(self, config):
        self.output = config.Output.STATIC_LIB

        self.third_parties = [
            'googletest',
        ]
    
    def pre_build(self):
        print('pre build: reflection')

    def post_build(self):
        print('post build: reflection')
