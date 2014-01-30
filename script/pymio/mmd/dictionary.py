import csv
import os

class Dictionary:
    initialized = False

    @classmethod
    def init(cls):
        cls.to_english_map = {}
        cls.to_japanese_map = {}
        try:
            d = os.path.dirname
            csvreader = csv.reader(file(os.path.join(d(d(d(d(__file__)))), 'data/mmd/dictionary.txt')))
            for row in csvreader:
                japanese_name = row[0].decode('shift_jis').encode('UTF-8')
                english_name = row[1].decode('shift_jis').encode('UTF-8')
                cls.to_english_map[japanese_name] = english_name
                cls.to_japanese_map[english_name] = japanese_name 
            cls.initialized = True
        except Exception, msg:
            print msg
            
    @classmethod
    def to_english(cls, name):
        if not cls.initialized:
            cls.init()
        if cls.to_english_map.has_key(name):
            return cls.to_english_map[name]
        else:                        
            return name.decode('UTF-8').encode('unicode_escape')
    
    @classmethod
    def to_japanese(cls, name):
        if not cls.initialized:
            cls.init()
        if cls.to_japanese_map.has_key(name):
            return cls.to_japanese_map[name]
        else:
            return name.decode('unicode_escape').encode('UTF-8')
        
if __name__=="__main__":    
    print Dictionary.to_japanese("tiptoe.L")