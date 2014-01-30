import cStringIO

class PrettyPrinter:
    def __init__(self, spaces=0, tab=2):
        self.output = cStringIO.StringIO()        
        self.spaces = spaces
        self.tab = tab
        
    def append(self, string):
        self.output.write(string)        
        
    def append_indented(self, string):
        self.append(" " * self.spaces + string)        
    
    def newline(self):
        self.append("\n")        
    
    def append_indented_line(self, string):
        self.append(" " * self.spaces + string + "\n")        
        
    def indent(self):
        self.spaces += self.tab
        
    def outdent(self):
        self.spaces -= self.tab
        if (self.spaces < 0):
            self.space = 0
            
    def append_indentation(self):
        self.append(" " * self.spaces)
        
    def new_indented_line(self):
        self.append("\n" + " " * self.spaces)
        
    def get_output(self):
        return self.output.getvalue()