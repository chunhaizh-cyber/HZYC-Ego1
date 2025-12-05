import tkinter as tk
from tkinter import ttk
import math

class Calculator:
    def __init__(self, root):
        self.root = root
        self.root.title("计算器")
        self.root.geometry("400x600")
        self.root.resizable(False, False)
        self.root.configure(bg='#f0f0f0')
        
        # 设置样式
        self.setup_styles()
        
        # 当前显示的数字
        self.current = "0"
        # 上一个操作数
        self.previous = ""
        # 当前操作符
        self.operator = ""
        # 是否刚完成计算
        self.reset_display = False
        
        self.create_widgets()
    
    def setup_styles(self):
        """设置按钮样式"""
        self.style = ttk.Style()
        
        # 数字按钮样式
        self.style.configure("Number.TButton",
                           font=("Segoe UI", 16),
                           padding=10)
        
        # 操作符按钮样式
        self.style.configure("Operator.TButton",
                           font=("Segoe UI", 16),
                           padding=10)
        
        # 等号按钮样式
        self.style.configure("Equals.TButton",
                           font=("Segoe UI", 16, "bold"),
                           padding=10)
        
        # 功能按钮样式
        self.style.configure("Function.TButton",
                           font=("Segoe UI", 14),
                           padding=10)
    
    def create_widgets(self):
        """创建界面组件"""
        # 主框架
        main_frame = tk.Frame(self.root, bg='#f0f0f0')
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # 显示屏框架
        display_frame = tk.Frame(main_frame, bg='#ffffff', relief=tk.SUNKEN, bd=2)
        display_frame.pack(fill=tk.X, pady=(0, 10))
        
        # 历史显示（小字）
        self.history_var = tk.StringVar()
        self.history_label = tk.Label(display_frame, 
                                    textvariable=self.history_var,
                                    font=("Segoe UI", 12),
                                    bg='#ffffff',
                                    fg='#666666',
                                    anchor='e',
                                    justify='right')
        self.history_label.pack(fill=tk.X, padx=10, pady=(10, 0))
        
        # 主显示屏
        self.display_var = tk.StringVar(value="0")
        self.display = tk.Label(display_frame,
                              textvariable=self.display_var,
                              font=("Segoe UI", 28, "bold"),
                              bg='#ffffff',
                              fg='#000000',
                              anchor='e',
                              justify='right')
        self.display.pack(fill=tk.X, padx=10, pady=(0, 10))
        
        # 按钮框架
        button_frame = tk.Frame(main_frame, bg='#f0f0f0')
        button_frame.pack(fill=tk.BOTH, expand=True)
        
        # 创建按钮布局
        self.create_buttons(button_frame)
    
    def create_buttons(self, parent):
        """创建按钮布局"""
        # 按钮配置：(文本, 行, 列, 列跨度, 样式, 命令)
        buttons = [
            # 第一行：功能按钮
            ("MC", 0, 0, 1, "Function.TButton", lambda: self.memory_clear()),
            ("MR", 0, 1, 1, "Function.TButton", lambda: self.memory_recall()),
            ("M+", 0, 2, 1, "Function.TButton", lambda: self.memory_add()),
            ("M-", 0, 3, 1, "Function.TButton", lambda: self.memory_subtract()),
            
            # 第二行：清除和退格
            ("CE", 1, 0, 1, "Function.TButton", lambda: self.clear_entry()),
            ("C", 1, 1, 1, "Function.TButton", lambda: self.clear_all()),
            ("⌫", 1, 2, 1, "Function.TButton", lambda: self.backspace()),
            ("±", 1, 3, 1, "Function.TButton", lambda: self.toggle_sign()),
            
            # 第三行：数学函数
            ("√", 2, 0, 1, "Function.TButton", lambda: self.sqrt()),
            ("x²", 2, 1, 1, "Function.TButton", lambda: self.square()),
            ("1/x", 2, 2, 1, "Function.TButton", lambda: self.reciprocal()),
            ("÷", 2, 3, 1, "Operator.TButton", lambda: self.set_operator("÷")),
            
            # 第四行：数字7-9和乘法
            ("7", 3, 0, 1, "Number.TButton", lambda: self.append_number("7")),
            ("8", 3, 1, 1, "Number.TButton", lambda: self.append_number("8")),
            ("9", 3, 2, 1, "Number.TButton", lambda: self.append_number("9")),
            ("×", 3, 3, 1, "Operator.TButton", lambda: self.set_operator("×")),
            
            # 第五行：数字4-6和减法
            ("4", 4, 0, 1, "Number.TButton", lambda: self.append_number("4")),
            ("5", 4, 1, 1, "Number.TButton", lambda: self.append_number("5")),
            ("6", 4, 2, 1, "Number.TButton", lambda: self.append_number("6")),
            ("-", 4, 3, 1, "Operator.TButton", lambda: self.set_operator("-")),
            
            # 第六行：数字1-3和加法
            ("1", 5, 0, 1, "Number.TButton", lambda: self.append_number("1")),
            ("2", 5, 1, 1, "Number.TButton", lambda: self.append_number("2")),
            ("3", 5, 2, 1, "Number.TButton", lambda: self.append_number("3")),
            ("+", 5, 3, 1, "Operator.TButton", lambda: self.set_operator("+")),
            
            # 第七行：0、小数点和等号
            ("0", 6, 0, 2, "Number.TButton", lambda: self.append_number("0")),
            (".", 6, 2, 1, "Number.TButton", lambda: self.append_number(".")),
            ("=", 6, 3, 1, "Equals.TButton", lambda: self.calculate()),
        ]
        
        # 配置网格权重
        for i in range(7):
            parent.grid_rowconfigure(i, weight=1)
        for i in range(4):
            parent.grid_columnconfigure(i, weight=1)
        
        # 创建按钮
        for text, row, col, colspan, style, command in buttons:
            btn = ttk.Button(parent, text=text, style=style, command=command)
            btn.grid(row=row, column=col, columnspan=colspan, 
                    sticky="nsew", padx=2, pady=2)
        
        # 内存变量
        self.memory = 0
    
    def append_number(self, num):
        """添加数字到显示屏"""
        if self.reset_display or self.current == "0":
            if num == ".":
                self.current = "0."
            else:
                self.current = num
            self.reset_display = False
        else:
            if num == "." and "." in self.current:
                return  # 防止多个小数点
            self.current += num
        
        self.update_display()
    
    def set_operator(self, op):
        """设置操作符"""
        if self.operator and not self.reset_display:
            self.calculate()
        
        self.previous = self.current
        self.operator = op
        self.reset_display = True
        self.update_history()
    
    def calculate(self):
        """执行计算"""
        if not self.operator or not self.previous:
            return
        
        try:
            prev = float(self.previous)
            curr = float(self.current)
            
            if self.operator == "+":
                result = prev + curr
            elif self.operator == "-":
                result = prev - curr
            elif self.operator == "×":
                result = prev * curr
            elif self.operator == "÷":
                if curr == 0:
                    self.current = "错误：除零"
                    self.update_display()
                    return
                result = prev / curr
            
            # 格式化结果
            if result == int(result):
                self.current = str(int(result))
            else:
                self.current = str(result)
            
            self.update_display()
            self.history_var.set(f"{self.previous} {self.operator} {curr} =")
            self.operator = ""
            self.previous = ""
            self.reset_display = True
            
        except Exception as e:
            self.current = "错误"
            self.update_display()
    
    def clear_all(self):
        """清除所有"""
        self.current = "0"
        self.previous = ""
        self.operator = ""
        self.reset_display = False
        self.history_var.set("")
        self.update_display()
    
    def clear_entry(self):
        """清除当前输入"""
        self.current = "0"
        self.update_display()
    
    def backspace(self):
        """退格"""
        if len(self.current) > 1:
            self.current = self.current[:-1]
        else:
            self.current = "0"
        self.update_display()
    
    def toggle_sign(self):
        """切换正负号"""
        if self.current != "0":
            if self.current.startswith("-"):
                self.current = self.current[1:]
            else:
                self.current = "-" + self.current
        self.update_display()
    
    def sqrt(self):
        """平方根"""
        try:
            num = float(self.current)
            if num < 0:
                self.current = "错误：负数"
            else:
                result = math.sqrt(num)
                self.current = str(result) if result != int(result) else str(int(result))
            self.update_display()
            self.reset_display = True
        except:
            self.current = "错误"
            self.update_display()
    
    def square(self):
        """平方"""
        try:
            num = float(self.current)
            result = num * num
            self.current = str(result) if result != int(result) else str(int(result))
            self.update_display()
            self.reset_display = True
        except:
            self.current = "错误"
            self.update_display()
    
    def reciprocal(self):
        """倒数"""
        try:
            num = float(self.current)
            if num == 0:
                self.current = "错误：除零"
            else:
                result = 1 / num
                self.current = str(result) if result != int(result) else str(int(result))
            self.update_display()
            self.reset_display = True
        except:
            self.current = "错误"
            self.update_display()
    
    def memory_clear(self):
        """清除内存"""
        self.memory = 0
    
    def memory_recall(self):
        """调用内存"""
        self.current = str(self.memory) if self.memory != int(self.memory) else str(int(self.memory))
        self.update_display()
        self.reset_display = True
    
    def memory_add(self):
        """内存加"""
        try:
            self.memory += float(self.current)
        except:
            pass
    
    def memory_subtract(self):
        """内存减"""
        try:
            self.memory -= float(self.current)
        except:
            pass
    
    def update_display(self):
        """更新显示屏"""
        # 限制显示长度
        if len(self.current) > 15:
            self.display_var.set(self.current[:15] + "...")
        else:
            self.display_var.set(self.current)
    
    def update_history(self):
        """更新历史显示"""
        if self.previous and self.operator:
            self.history_var.set(f"{self.previous} {self.operator}")

def main():
    root = tk.Tk()
    app = Calculator(root)
    
    # 键盘绑定
    def on_key_press(event):
        key = event.char
        if key.isdigit():
            app.append_number(key)
        elif key == '.':
            app.append_number('.')
        elif key == '+':
            app.set_operator('+')
        elif key == '-':
            app.set_operator('-')
        elif key == '*':
            app.set_operator('×')
        elif key == '/':
            app.set_operator('÷')
        elif key == '\r' or key == '=':  # Enter键或等号
            app.calculate()
        elif event.keysym == 'BackSpace':
            app.backspace()
        elif event.keysym == 'Escape':
            app.clear_all()
        elif event.keysym == 'Delete':
            app.clear_entry()
    
    root.bind('<Key>', on_key_press)
    root.focus_set()  # 确保窗口可以接收键盘事件
    
    root.mainloop()

if __name__ == "__main__":
    main()