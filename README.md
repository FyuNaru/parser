# LALR Parser

### 项目结构
```
├─3rd // 空
│  
├─bin  // exe 文件
│  ├─Debug
│  └─Release
├─src
│  ├─cent.sln // 打开
│  ├─codes // 空
│  ├─log   // parsertest 输出的日志文件、分析表的 html 文件
│  ├─parser // 主项目
|  |  ├─parser.cpp 主程序
│  │  ├─include
│  │  └─src
│  └─parsertest // 单元测试
└─temp
    
```

### 使用的文法
```
P' -> Program
Program -> DeclarationList
DeclarationList -> DeclarationList Declaration | Declaration
Declaration -> VarDeclaration | FuncDeclaration

// support type
Type -> int | bool | double

// variable declaration
VarDeclaration -> Type IdList ;
IdList -> IdList VarInit | VarInit
VarInit -> VarId | VarId = SimpleExpr
VarId -> id | id [ const ]

// function declaration
FuncDeclaration -> Type id ( Params ) CompositeStmt
Params -> ParamList | ε
ParamList -> ParamList , P | P
P -> Type ParamId
ParamId -> id | id [ ]

// statements
CompositeStmt ->  { StmtList }
StmtList -> StmtList Stmt | ε
Stmt -> WhileStmt | Expr ; | IfStmt | CompositeStmt | VarDeclaration | ;
IfStmt -> if ( Expr ) Stmt ElseStmt
ElstStmt -> else Stmt | ε
WhileStmt -> while ( Expr ) Stmt

// Expressions
Expr -> Mutable = Expr | SimpleExpr
Mutable -> Mutable [ Expr ] | id
SimpleExpr -> SimpleExpr or AndExpr | AndExpr
AndExpr -> AndExpr and NotExpr | NotExpr
NotExpr -> not NotExpr | RelationExpr
RelationExpr -> SumExpr Relop SumExpr | SumExpr
SumExpr -> SumExpr Sumop MultiExpr | MultiExpr
MultiExpr -> MultiExpr Multiop UExpr | UExpr
UExpr -> Uop UExpr | Factor
Factor -> Immutable | Mutable
Immutable -> ( Expr ) | Call | const
Call -> id ( Args )
Args -> ArgList | ε
ArgList -> ArgList , Expr | Expr

// operators
Relop -> == | != | > | < | >= | <=
Sumop -> + | -
Multiop -> * | / | %
Uop -> * | -	
```

### ToDo
- 先序输出语法分析树
- 错误处理
- 界面
- FOLLOW集 (?)