

# Setup Conda in VS Code
Ref: https://medium.com/analytics-vidhya/efficient-way-to-activate-conda-in-vscode-ef21c4c231f2

 - In VS Code, open 'Workspace Settings (JSON)" add: 
     {    
    "python.pythonPath": "C:\\\Users\\\\[user-name]\\\anaconda3\\\python.exe",
    "python.terminal.activateEnvironment": true,    
    }
  - Note: \\\ is used in windows for path
  - If terminal doesn't allow conda activate, do 'conda init'
    
