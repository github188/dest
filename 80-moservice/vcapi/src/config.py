from configparser import ConfigParser

#default configs
configs = {
    'mq': {
        'ip': '127.0.0.1',
        'port': 5672,
        'username': 'dev',
        'password': 'dev',
    },
    'apdb': {
        'ip': '127.0.0.1',
        'username': 'kedacom',
        'password': 'KedaMysql16#',
    },
    'redis': {
        'ip': '127.0.0.1',
        'port': 6379,
        'password': 'KedaRedis16',
    },
    'auth': {
        'ip': '127.0.0.1',
    },
    'amc-api': {
        'ip': '127.0.0.1',
    },
    'local': {
        'port': 8070,
    },
}

def load(filename):
    p = ConfigParser()
    p.read(filename)
    sections = p.sections()
    for v in sections:
        for k in p[v]:
            configs[v][k] = p[v][k]
