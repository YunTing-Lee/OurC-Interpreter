# include <iostream>
# include <stdio.h>
# include <vector>
# include <string>
# include <cstring>
# include <cstdlib>
# include <iomanip>
# include <stack>
# include <sstream>
# include <map>
# include <stdlib.h>

using namespace std ;


enum TokenType
{
    IDENT = 25678, NUM = 85214, CONSTANT = 15645,
    INT = 54896, FLOAT = 15462, CHAR = 35415, BOOL = 97845, STRING = 45231, VOID = 68472,  //  ���O
    IF = 75232, ELSE = 56324, WHILE = 32512, DO = 74312, RETURN = 74526,
    LEFT_SAMLL_PAREN = 13876, RIGHT_SAMLL_PAREN = 43764, LEFT_MID_PAREN = 95124,
    RIGHT_MID_PAREN = 23654, LEFT_BIG_PAREN = 53641, RIGHT_BIG_PAREN = 84512,
    PLUS = 58719, MINUS = 79425, MULTIPLICAION = 45732, DIVISION = 69727, PERCENT = 79153,
    CARET = 84163, GREATER = 91279, LESS = 11564, GE = 96523, LE = 25863, EQUAL = 34715,
    NOTEQUAL = 75623, ONE_AND = 21561, ONE_OR = 84562, ASSIGN = 98627, NOT = 74161,
    AND = 45631, OR = 65412, PE = 22489, ME = 97992, TE = 113698, DE = 74235,
    RE = 33685, PP = 55741, MM = 78725, RS = 84187, LS = 97872, SEMICOLON = 24631, COMMA = 44974,
    QUES = 46852, COLON = 14563, CIN = 95841, COUT = 45132, SIGN = 84135,
    SINGLE_QUOTE = 15519, QUOTE = 74513, DOT = 52336,
    UNRECOG = 41652, COMMENT = 20584, QUIT = 68541,
    INPUTBUFFERAPPEND = 12187, ENDINPUTBUFFERAPPEND = 94163,
    NOTOKEN = 40659,
    IDENT_ARRAY = 23564
} ;


int gLine = 1 ;
int gColumn = 0 ;
int gCommandLine = 1 ;
bool IsDigit( string str ) ;
bool IsAlpha( char ch ) ;

struct Token{
  string tokenStr ;
  TokenType type ;
};

class Exception {
public:
  Exception() {  }

}; //  class Expception


class LexicalError : public Exception {

public:

  LexicalError( char ch, int line, char nextChar ) {
    string chStr ;
    chStr += ch ;
    if ( nextChar == '\n' )
      line-- ;
    cout << "Line " << line << " : unrecognized token with first char '" << chStr << "'" << endl ;
  } // LexicalError()

};

class SyntacticError : public Exception {
public:

  SyntacticError( string token, int line, char nextChar  ) {
    if ( nextChar == '\n' )
      line-- ;
    cout << "Line " << line << " : unexpected token '" << token << "'" << endl ;
  } // SyntacticError()
};

class UndefinedIdError : public Exception {
public:

  UndefinedIdError( string token, int line, char nextChar  ) {
    if ( nextChar == '\n' )
      line-- ;
    cout << "Line " << line << " : undefined identifier '" + token + "'" << endl ;
  } // UndefinedIdError()
};

class DividedByZero : public Exception {

public:
  DividedByZero() {
    cout << "Error" << endl ;
  } // DividedByZero()
};

class EOFError : public Exception {

public:
  EOFError() {
    cout << "Error" << endl ;
  } // EOFError()
};

class NoInputError : public Exception {

public:
  NoInputError( string str, bool isArray, int indexNum ) {
    if ( ! isArray ) // ���Oarray
      cout << "Run-time error : No input when cin '" << str << "'!" << endl ;
    else // �Oarray
      cout << "Run-time error : No input when cin '" << str << "[ " << indexNum << " ]'!" << endl ;
  } // NoInputError()
};

class InputTypeError : public Exception {

public:
  InputTypeError( string str, bool isArray, int indexNum ) {
    if ( ! isArray ) // ���Oarray
      cout << "Run-time error : Input error when cin '" << str << "'!" << endl ;
    else // �Oarray
      cout << "Run-time error : Input error when cin '" << str << "[ " << indexNum << " ]'!" << endl ;
  } // InputTypeError()
};

class ArrayIndexOutofRangeError : public Exception {
public:
  ArrayIndexOutofRangeError( string arrayName, string indexName, int index ) {
    cout << "Runtime error : array '" << arrayName << "' index '" << indexName
         << "' = " << index <<  " out of range!" << endl ;
  } // ArrayIndexOutofRangeError()
};

struct Variable{
  int level ; // �O�ĴX�h���ܼ�
  string type ;
  string name ;
  vector<string> value ; // �s���ܼƭȡA���Oarray size��1
  bool isArray ;
  int arraySize ;
  bool passByReference ;
  // Variable * referenceVar ; // �ѦҨ���@���ܼ�
  string referenceVar ;
  int referenceIndex ;
  int referenceLevel ;
  string referenceFunction ; // �ѦҨ�Y��function���Y�ӰѼ�
};

struct Function { // �@��function����T
  string name ;
  string returnType ;
  vector<Variable> parameterList ; // ���vector �]����map�|�۰ʫ��W�r�ƧǡA���Ѽƶ��Ǥ�����
  vector< map<string, Variable > > funcDefineVar ;
  vector<Token> tokenList ; // �s��o�Ө�Ʃw�q���Ҧ�token
  vector<Token> body ;
  int level ;
};

class Information { // �@��command����T
public :
  bool mIsDeclaration ; // �ŧi�ܼ�
  bool mIsFunction ;  // �ŧifunction
  bool mIsStatement ;
  bool mIsSystemFunction ; // �t�ιw�]��ơADone(), ListVariables()...
  vector<Token> mTokenList ; // �s��o��command���Ҧ�token
  // int mLevel ; // �o��command�{�b�b���h�ơA����b��0�h
  vector<Variable> mParameterList ; // �s��ƶǤJ���Ѽ�
  vector< map<string, Variable > > mFuncDefineVar ;
  int mFuncLevel ;
  string mfuncName ;

  Information() {
    mIsDeclaration = false ;
    mIsFunction = false ;
    mIsStatement = false ;
    mIsSystemFunction = false ;
    mTokenList.clear() ;
    mParameterList.clear() ;
    mFuncDefineVar.clear() ;
    mFuncLevel = 0 ;
    mfuncName = "" ;
  } // Information()

  void Initialize() {
    mFuncLevel = 0 ;
    mIsDeclaration = false ;
    mIsFunction = false ;
    mIsStatement = false ;
    mIsSystemFunction = false ;
    mTokenList.clear() ;
    mParameterList.clear() ;
    mFuncDefineVar.clear() ;
    mfuncName = "" ;
  } // Initialize()

}; // class Information

class C_Interpreter{

public :

  C_Interpreter() {
    mNextChar = ' ' ;
    mLevel = 0 ;
  } // C_Interpreter()


  char mNextChar ;
  vector<Token> mEvalToken ;
  string mPeekWhat ; // �Ψ��x�s���ݨ쪺�r�A�nputback�ɥ�
  int mCommandLine ; // �ΨӰO���O�{�b�o�ӫ��O���ĴX��A��XError�ɥ�
  vector<Information> mCommandList ; // �Ҧ���command(�S�Ψ�)
  string mInputBuffer ; // cin�nŪ�J��input
  Information mCurrentCommand ; // �ثecommand����T
  int mLevel ;

  map<string, Function> mFunctionTable ; // ListFunction�ϥΡA�s�Ҧ�function
  // �s�Ҧ����ܼơAmVarTable[0]->��0�h�i�Ϊ��ܼ� mVarTable[1]->��1�h�i�Ϊ��ܼ�
  vector< map<string, Variable > > mVarTable ;
  void CatchError() ;

  void GetToken( TokenType & type, string & tokenStr, bool isPeek, bool & hasComment ) ;
  void PeekToken( TokenType & type, string & tokenStr  ) ;

  string GetAlphaStartToken( bool isPeek ) ;
  void WhatAlphaStartType( TokenType & type, string tokenStr )  ;
  string GetAQuoteSatarToken( bool isPeek ) ;
  string GetDoubleQuoteSatarToken( bool isPeek ) ;

  void GetNextChar( bool isPeek ) ;
  void SkipAllWhite( bool isPeek ) ;
  void SkipWhiteAndTabAndComment() ;
  string GetIndent( bool isPeek ) ;
  string GetNum( bool isPeek ) ;
  string GetMark( TokenType & type, bool isPeek  ) ;
  void ReadUntilEnter( bool isPeek ) ;

  void IsIdDefined( string id, Variable & varInfo ) ;
  void IsFunctionDefined( string functionName ) ;

  void Parser() ;
  void User_Input( bool & isDone ) ;
  void Done() ;
  void ListVariableOrFunction( string & identStr ) ;
  void ListAllVariablesOrFunctions() ;
  void Definition() ;
  bool Type_specifier( TokenType type ) ;
  void Function_definition_or_declarators( string identType, string firstIdentName ) ;
  void Rest_of_declarators(  string identType, string firstIdentName ) ;
  void Function_definition_without_ID() ;
  void Formal_parameter_list() ;
  void Compound_statement() ;
  void Declaration() ;
  void Statement() ;
  bool IsBasicExpressionBeginToken( TokenType type ) ;
  void Expression() ;
  void Basic_expression() ;
  void Rest_of_Identifier_started_basic_exp() ;
  void Rest_of_PPMM_Identifier_started_basic_exp() ;
  bool IsSign( TokenType type ) ;
  void Actual_parameter_list() ;
  bool Assignment_operator( TokenType type ) ;
  void Romce_and_romloe() ;
  void Rest_of_maybe_logical_OR_exp() ;
  void Maybe_logical_AND_exp() ;
  void Rest_of_maybe_logical_AND_exp() ;
  void AND_Maybe_bit_OR_exp_RepeatZeroToMany() ;
  void Maybe_bit_OR_exp() ;
  void Rest_of_maybe_bit_OR_exp() ;
  void ONEOR_Maybe_bit_ex_OR_exp_RepeatZeroToMany() ;
  void Maybe_bit_ex_OR_exp() ;
  void Rest_of_maybe_bit_ex_OR_exp() ;
  void CARET_Maybe_bit_AND_exp_RepeatZeroToMany() ;
  void Maybe_bit_AND_exp() ;
  void Rest_of_maybe_bit_AND_exp() ;
  void ONEAND_Maybe_equality_exp_RepeatZeroToMany() ;
  void Maybe_equality_exp() ;
  void Rest_of_maybe_equality_exp() ;
  void EQorNEQ_Maybe_relational_exp_RepeatZeroToMany() ;
  void Maybe_relational_exp() ;
  void Rest_of_maybe_relational_exp() ;
  void LorGorLEorGE_Maybe_shift_exp_RepeatZeroToMany() ;
  void Maybe_shift_exp() ;
  void Rest_of_maybe_shift_exp() ;
  void LSorRS_Maybe_additive_exp_RepeatZeroToMany() ;
  void Maybe_additive_exp() ;
  void Rest_of_maybe_additive_exp() ;
  void PLUSorMINUS_Maybe_mult_exp_RepeatZeroToMany() ;
  void Maybe_mult_exp() ;
  void Rest_of_maybe_mult_exp() ;
  void Unary_exp() ;
  void Signed_unary_exp() ;
  void Unsigned_unary_exp() ;
  void Semicolon() ;
  void Optional_MidParen_Constant( bool & isArray, int & arraySize ) ;
  void MidParenExpression() ;
  void SmallParenExpression() ;
  void SmallParenOptionalActual_parameter_list() ;

  void StoreFunction( string returnType, string identName )  ;
  void StoreVariable( Variable var ) ;
  void StoreLocalVariable( vector<Variable> localVar ) ;
  void StoreFunctionLocalVar( vector<Variable> funcVar ) ;
  void ListVariable( string variableName ) ;
  void ListFunction( string functionName )  ;
  void PrintSpace( int level ) ;
  void PrintFunction( int & i, map<string, Function>::iterator & iter, int & level, bool & isStart ) ;
  void PrintIfOrWhile( int & i, map<string, Function>::iterator & iter, int & level, bool & isStart ) ;
  void PrintStatement( int & i, map<string, Function>::iterator & iter, int & level, bool & isStart ) ;
  void PrintParenExpression( int & i, map<string, Function>::iterator & iter, int & level, bool & isStart ) ;
  void SystemSupportFunction( bool & isDone ) ;

  void InputBufferAppend() ; // project3�s�W
  string ReadInputFromBuffer( string type, string identName, bool isArray, int indexNum  ) ;
  void SkipWhiteSpaceInBuffer( int & howManyWhiteSpace ) ;
  void InitializeVariable( Variable & var ) ;

  void Evaluate() ;
  void Evaluate_Statement( vector<Token> stat, int & i, string caller, Token & result ) ;
  void Evaluate_Expression( vector<Token> stat, stack<Token> & operandStack, stack<string> & operatorStack,
                             string needType, string caller ) ;
  void ClearStack( stack<Token> & operandStack, stack<string> & operatorStack ) ;

  void Evaluate_Basic_Expression( vector<Token> calToken, stack<Token> & operandStack,
                                  stack<string> & operatorStack, string needType, string caller ) ;

  void Identifier( string identName, vector<Token> calToken, int & i, Token preToken, string & needType,
                   stack<Token> & operandStack, stack<string> & operatorStack, Variable & identInfo,
                   int & indexNum, string caller ) ;
  void PPMM( Token ppmm, Variable & identInfo, int indexNum, string caller ) ;
  void SignIdent( Token sign, Variable & identInfo, int indexNum, string & signValue ) ;
  void RightParen( stack<Token> & operandStack, stack<string> & operatorStack, string needType ) ;
  void Operator( string currentOp, stack<Token> & operandStack, stack<string> & operatorStack,
                 string needType, string caller ) ;
  int OperatorPriority( string op ) ;
  void CalculateStack( stack<Token> & operandStack, stack<string> & operatorStack, string needType,
                       string caller ) ;
  void CalRestInStack( stack<Token> & operandStack, stack<string> & operatorStack, string needType,
                       string caller ) ;
  void Calculate( string op, Token token1, Token token2, Token & result, string & needType,
                   string caller ) ;
  void GetTokenValue( Token token, string & type, string & value, string caller ) ;
  bool IsNum( string str, bool & isFloat ) ;
  void GetVariableInfo( string identName, Variable & var, string caller, int & index,
                        bool & funcPara, bool & byRef, bool & byFuncRef,
                        string & referFunc, string & referVar ) ;
  void UpdateVariable( string identName, Variable updateVar, string caller ) ;
  void Assign( Token left, string leftType, string rightType, string assignValue, Token & result,
               string caller ) ;
  void CalArrayIndexExpression( vector<Token> calToken, int & i, string & indexExp, int & indexNum,
                                string caller ) ;
  void Cin( vector<Token> calToken, int & i, string caller ) ;
  void Cout( vector<Token> calToken, int & i, stack<Token> & operandStack, stack<string> & operatorStack,
             string & needType, string caller ) ;
  void PrintCout( Token coutToken, stack<Token> & operandStack, string caller ) ;
  void Sign( vector<Token> calToken, int & i, stack<Token> & operandStack, stack<string> & operatorStack,
             string & needType, string caller ) ;
  void QuestionMark( vector<Token> calToken, int & i, stack<Token> & operandStack,
                     stack<string> & operatorStack, string & needType, string caller ) ;
  void GetInParenExp( vector<Token> calToken, int & i, vector<Token> & parenExp ) ;
  void NumToBool( string & value ) ;
  void InCompoundDeclaration( vector<Token> stat, int & i, string caller ) ;
  void GetIfWhileStatement( vector<Token> stat, int & i, vector<Token> & doStat ) ;
  void PrintVar() ; // debug �ΡA�L�X�Ҧ��ܼ�

  void GetFunctionInfo( string identName, Function & func ) ;
  void CallFunction( string funcName, vector<Token> calToken, int & i, string caller,
                     stack<Token> & operandStack ) ;
  void FunctionDone( string caller ) ;
  void UpdateFunction( string identName, Function updateFunc ) ;
  void StoreFunctionVar( Function & func, vector<Variable> funcVar ) ;
}; // class C_Interpreter

int gTestNum ;

int main() {
  C_Interpreter cInter ;
  char ch ;
  cin >> gTestNum ;
  cin.get( ch ) ;
  cout << "Our-C running ..." << endl ;
  cInter.Parser() ;
  cout << "Our-C exited ..." << endl ;

  return 0;
} // main()

// / *************************  Lexical Analysis ***************************///

bool IsDigit( char ch ) {
  return ( ch >= '0' && ch <= '9' ) ;
} // IsDigit()

bool IsAlpha( char ch ) {
  return ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) ;
} // IsAlpha()

string NumToString( double floatVal, bool needfloat ) {
  char result[50] ;

  if ( needfloat )
    sprintf( result, "%f", floatVal ) ;
  else {
    int intVal = floatVal ;
    sprintf( result, "%d", intVal ) ;
  } // else

  string str ;
  for ( int i = 0 ; result[i] != '\0' ; i++ ) {
    str += result[i] ;
  } // for

  return str ;

} // NumToString()

void C_Interpreter :: SkipWhiteAndTabAndComment() {

  while ( mNextChar == ' ' || mNextChar == '\t' || ( mNextChar == '/' && cin.peek() == '/' ) ) {
    if ( ( mNextChar == '/' && cin.peek() == '/' ) ) { // �O����
      ReadUntilEnter( false ) ;
    } // if
    else
      GetNextChar( false ) ;
  } // while

} // C_Interpreter::SkipWhiteAndTabAndComment()


void C_Interpreter :: SkipAllWhite( bool isPeek ) {
// �YisPeek��true�B�쥻mNextChar�O�ťաA�~�N�s���o���O�ťժ�mNextChar�[��mPeekWhat

  if ( mNextChar == ' ' || mNextChar == '\t' || mNextChar == '\n'  ) {
    while ( mNextChar == ' ' || mNextChar == '\t' || mNextChar == '\n' ) {  // �O�ťժ��N�������L�A�Y�Opeek����]���ΦA��^�h
      GetNextChar( false ) ;
    } // while

    if ( isPeek )
      mPeekWhat += mNextChar ;
  } // if
} // C_Interpreter::SkipAllWhite()

void C_Interpreter ::  ReadUntilEnter( bool isPeek ) {
  while ( mNextChar != '\n' )
    GetNextChar( isPeek ) ;
} // C_Interpreter::ReadUntilEnter()

void C_Interpreter :: GetNextChar( bool isPeek ) { // ���o�U�@��char�A�YisPeek��true�h�N���쪺char�[�bmPeekWhat

  if ( cin.peek() != EOF )
    mNextChar = cin.get();
  else
    throw new EOFError() ;

  if ( isPeek )
    mPeekWhat += mNextChar ;

  if ( mNextChar == '\n' && ! isPeek ) {
    mCommandLine++ ;
    gLine++ ;
    gColumn = 0 ;
  } // if
  else
    gColumn++ ;
} // C_Interpreter::GetNextChar()

void C_Interpreter :: WhatAlphaStartType( TokenType & type, string tokenStr ) {

  if ( tokenStr == "int" )
    type = INT ;
  else if ( tokenStr == "float" )
    type = FLOAT ;
  else if ( tokenStr == "char" )
    type = CHAR ;
  else if ( tokenStr == "bool" )
    type = BOOL ;
  else if ( tokenStr == "string" )
    type = STRING ;
  else if ( tokenStr == "void" )
    type = VOID ;
  else if ( tokenStr == "if" )
    type = IF ;
  else if ( tokenStr == "else" )
    type = ELSE ;
  else if ( tokenStr == "while" )
    type = WHILE ;
  else if ( tokenStr == "do" )
    type = DO ;
  else if ( tokenStr == "return" )
    type = RETURN ;
  else if ( tokenStr == "cin" )
    type = CIN ;
  else if ( tokenStr == "cout" )
    type = COUT ;
  else if ( tokenStr == "true" || tokenStr == "false" )
    type = CONSTANT ;
  else if ( tokenStr == "InputBufferAppend" ) // project3�s�W
    type = INPUTBUFFERAPPEND ;
  else if ( tokenStr == "EndInputBufferAppend" )
    type = ENDINPUTBUFFERAPPEND ;
  else
    type = IDENT ;
} // C_Interpreter::WhatAlphaStartType()

void C_Interpreter :: GetToken( TokenType & type, string & tokenStr, bool isPeek, bool & hasComment ) {

  SkipAllWhite( isPeek ) ;

  if ( IsAlpha( mNextChar ) ) { // �^��r���}�Y
    // �i��O����r(int, float, char, bool, string,void, if, else, while, do, return, cin, cout)��ident
    tokenStr = GetAlphaStartToken( isPeek ) ;
    WhatAlphaStartType( type, tokenStr ) ;
  } // if
  else if ( IsDigit( mNextChar ) || ( mNextChar == '.' && IsDigit( cin.peek() ) ) ) { // NUM
    tokenStr = GetNum( isPeek ) ;
    type = CONSTANT ;
  } // else if
  else if ( mNextChar == '\'' ) { // ��޸�
    tokenStr = GetAQuoteSatarToken( isPeek ) ;
    type = CONSTANT ;
  } // else if
  else if ( mNextChar == '\"' ) { // ���޸�
    tokenStr = GetDoubleQuoteSatarToken( isPeek ) ;
    type = CONSTANT ;
  } // else if
  else if ( mNextChar == '/' && cin.peek() == '/' ) { // ����
    hasComment = true ;
    ReadUntilEnter( false ) ;
    mPeekWhat = "" ;
    GetToken( type, tokenStr, isPeek, hasComment ) ;
    return ;
  } // else if
  else {
    tokenStr = GetMark( type, isPeek ) ;
  } // else

  // �YisPeek��true�A�]���u�O���ݩҥH�����x�s�A����GetToken�N�|�s�F
  if ( ! isPeek ) {
    Token newToken ;
    newToken.tokenStr = tokenStr ;
    newToken.type = type ;
    mEvalToken.push_back( newToken ) ;
    mCurrentCommand.mTokenList.push_back( newToken ) ;
  } // if


} // C_Interpreter::GetToken()

void C_Interpreter :: PeekToken( TokenType & type, string & tokenStr  ) { // ���ݤU�@��token�A��Ҧ��h�ݪ�char�s�bmPeekWhat
  mPeekWhat = "" ; // �M��
  char originalNextChar = mNextChar ; // �]�I�sGetToken��i��|�s��s��mNextChar�A�ҥH���s�_�ӭ쥻��
  bool hasComment = false ;

  GetToken( type, tokenStr, true, hasComment ) ;

  if ( hasComment ) {
    mNextChar = mPeekWhat[0] ;
    for ( int i = mPeekWhat.size() - 1 ; i >= 1 ; i-- ) {
      cin.putback( mPeekWhat[i] ) ;
    } // for
  } // if
  else {
    mNextChar = originalNextChar ;
    for ( int i = mPeekWhat.size() - 1 ; i >= 0 ; i-- ) {
      cin.putback( mPeekWhat[i] ) ;
    } // for
  } // else if

} // C_Interpreter::PeekToken()

string C_Interpreter :: GetAQuoteSatarToken( bool isPeek ) { // ���o��޸��}��token
  string constant ;

  do {
    constant += mNextChar ;
    GetNextChar( isPeek ) ;
  } while ( mNextChar != '\'' && mNextChar != '\n' ) ; // Ū���޸��άOenter����


  if ( mNextChar == '\'' ) {
    constant = constant + mNextChar ;
    GetNextChar( isPeek ) ;
    return constant ;
  } // if
  else { // ���Ū���٨S���J���޸�-->error
    if ( isPeek )
      mCommandLine++;
    throw new LexicalError( '\'', mCommandLine, mNextChar ) ;
  } // end else
} // C_Interpreter::GetAQuoteSatarToken()

string C_Interpreter :: GetDoubleQuoteSatarToken( bool isPeek ) {
  // mNextChar is "
  string constant  ;
  do {
    constant += mNextChar ;
    GetNextChar( isPeek ) ;
  } while ( mNextChar != '"' && mNextChar != '\n' ) ;

  if ( mNextChar == '"' ) {
    constant = constant + mNextChar ;
    GetNextChar( isPeek ) ;
    return constant ;
  } // if
  else { // ���Ū���٨S���J���޸�-->error
    if ( isPeek )
      mCommandLine++;
    throw new LexicalError( '"', mCommandLine, mNextChar ) ;
  } // end else
} // C_Interpreter::GetDoubleQuoteSatarToken()

string C_Interpreter :: GetAlphaStartToken( bool isPeek ) { // ���o�^��r���}�Y��token

  string alphaToken ;
  while ( IsDigit( mNextChar ) || IsAlpha( mNextChar ) || mNextChar == '_' ) {
    alphaToken += mNextChar ;
    GetNextChar( isPeek ) ;
  } // while

  return alphaToken ;

} // C_Interpreter::GetAlphaStartToken()

string C_Interpreter :: GetNum( bool isPeek ) {

  string num ;
  bool hasDot = false ;

  while ( IsDigit( mNextChar ) || ( ! hasDot && mNextChar == '.' ) ) {
    num = num + mNextChar ;
    if ( mNextChar == '.' )
      hasDot = true ;

    GetNextChar( isPeek ) ;

  } // while

  return num ;
} // C_Interpreter::GetNum()

string C_Interpreter :: GetMark( TokenType & type, bool isPeek ) {

  string mark  ;
  mark = mark + mNextChar ;
  if ( mNextChar == '(' )
    type = LEFT_SAMLL_PAREN ;
  else if ( mNextChar == ')' )
    type = RIGHT_SAMLL_PAREN ;
  else if ( mNextChar == '[' )
    type = LEFT_MID_PAREN ;
  else if ( mNextChar == ']' )
    type = RIGHT_MID_PAREN ;
  else if ( mNextChar == '{' )
    type = LEFT_BIG_PAREN ;
  else if ( mNextChar == '}' )
    type = RIGHT_BIG_PAREN ;
  else if ( mNextChar == '+' && cin.peek() == '+' ) { // ++
    GetNextChar( isPeek ) ; // get '+'
    mark = mark + mNextChar ;
    type = PP ;
  } // else if
  else if (  mNextChar == '+' && cin.peek() == '=' ) {  // +=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = PE ;
  } // else if
  else if ( mNextChar == '+' ) // +
    type = PLUS ;
  else if ( mNextChar == '-' && cin.peek() == '-' ) { // --
    GetNextChar( isPeek ) ; // get '-'
    mark = mark + mNextChar ;
    type = MM ;
  } // else if
  else if (  mNextChar == '-' && cin.peek() == '=' ) { // -=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = ME ;
  } // else if
  else if ( mNextChar == '-' ) // -
    type = MINUS ;
  else if ( mNextChar == '*' && cin.peek() == '=' ) { // *=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = TE ;
  } // else if
  else if ( mNextChar == '*' ) // *
    type = MULTIPLICAION ;
  else if ( mNextChar == '/' && cin.peek() == '=' ) { // /=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = DE ;
  } // else if
  else if ( mNextChar == '/' ) // /
    type = DIVISION ;
  else if ( mNextChar == '%' && cin.peek() == '=' ) { // %=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = RE ;
  } // else if
  else if ( mNextChar == '%' ) // %
    type = PERCENT ;
  else if ( mNextChar == '^' )
    type = CARET ;
  else if ( mNextChar == '>' && cin.peek() == '>' ) { // >>
    GetNextChar( isPeek ) ; // get '>'
    mark = mark + mNextChar ;
    type = RS ;
  } // else if
  else if ( mNextChar == '>' && cin.peek() == '=' ) { // >=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = GE ;
  } // else if
  else if ( mNextChar == '>' ) // >
    type = GREATER ;
  else if ( mNextChar == '<' && cin.peek() == '<' ) { // <<
    GetNextChar( isPeek ) ; // get '<'
    mark = mark + mNextChar ;
    type = LS ;
  } // else if
  else if ( mNextChar == '<' && cin.peek() == '=' ) { //  <=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = LE ;
  } // else if
  else if ( mNextChar == '<' ) // <
    type = LESS ;
  else if ( mNextChar == '=' && cin.peek() == '=' ) { // ==
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = EQUAL ;
  } // else if
  else if ( mNextChar == '=' ) // =
    type = ASSIGN ;
  else if ( mNextChar == '!' && cin.peek() == '=' ) { // !=
    GetNextChar( isPeek ) ; // get '='
    mark = mark + mNextChar ;
    type = NOTEQUAL ;
  } // else if
  else if ( mNextChar == '!' )
    type = NOT ;
  else if ( mNextChar == '&' && cin.peek() == '&' ) { // &&
    GetNextChar( isPeek ) ; // get '&'
    mark = mark + mNextChar ;
    type = AND ;
  } // else if
  else if ( mNextChar == '&' ) // &
    type = ONE_AND ;
  else if ( mNextChar == '|' && cin.peek() == '|' ) { // ||
    GetNextChar( isPeek ) ; // get '|'
    mark = mark + mNextChar ;
    type = OR ;
  } // else if
  else if ( mNextChar == '|' ) // |
    type = ONE_OR ;
  else if (  mNextChar == ';' ) {
    type = SEMICOLON ;
  } // else if
  else if (  mNextChar == ',' )
    type = COMMA ;
  else if (  mNextChar == '?' ) {
    type = QUES ;
  } // else if
  else if (  mNextChar == ':' ) {
    type = COLON ;
  } // else if
  else {
    type = UNRECOG ;
    throw new LexicalError( mNextChar, mCommandLine, mNextChar ) ;
  } // else if

  GetNextChar( isPeek ) ;
  return mark ;
} // C_Interpreter::GetMark()

void C_Interpreter :: Parser() {

  bool isDone = false ;
  while ( ! isDone ) {
    try {
      mPeekWhat = "" ;
      mEvalToken.clear() ;
      mCurrentCommand.Initialize() ;
      cout << "> " ;
      mCommandLine = 1 ;
      User_Input( isDone ) ;
      if ( isDone )
        return ;

      SkipWhiteAndTabAndComment() ;
      Evaluate() ; //  ����o��command
      if ( mCurrentCommand.mIsStatement )
        cout << "Statement executed ..." << endl ;
    } // try
    catch ( LexicalError * error ) {
      CatchError() ;
    } // catch
    catch ( SyntacticError * error ) {
      CatchError() ;
    } // catch
    catch ( UndefinedIdError * error ) {
      CatchError() ;
    } // catch
    catch ( EOFError * error ) {
      isDone = true ;
    } // catch
    catch ( NoInputError * error ) { // run time error
      CatchError() ;
      cout << "Statement executed ..." << endl ;
    } // catch
    catch ( InputTypeError * error ) {  // run time error
      CatchError() ;
      cout << "Statement executed ..." << endl ;
    } // catch
    catch ( DividedByZero * error ) {  // run time error
      CatchError() ;
      cout << "Statement executed ..." << endl ;
    } // catch
    catch ( ArrayIndexOutofRangeError * error ) {
      CatchError() ;
      cout << "Statement executed ..." << endl ;
    } // catch

  } // while

  cout <<  "Our-C exited ..." << endl ;
} // C_Interpreter::Parser()


void C_Interpreter :: CatchError() {
  ReadUntilEnter( false ) ;
  mEvalToken.clear() ;
  mLevel = 0 ;
  while ( mVarTable.size() > 1 ) // �^��Ĥ@�h
    mVarTable.pop_back() ;

} // C_Interpreter::CatchError()

void C_Interpreter :: IsIdDefined( string id, Variable & varInfo ) {
// checkFunction��true��ܤ]�|�ˬdfunction�W�٦��S���@�˪�

  map<string, Variable>::iterator iter ;

  if ( mVarTable.size() != 0 ) { // ��ϰ��ܼƻP�����ܼƦ��S���@�˪�id�W��

    for ( int i = mVarTable.size() -1 ; i >= 0 ; i-- ) {
      iter = mVarTable[i].find( id ) ;
      if ( iter != mVarTable[i].end() ) {
        varInfo = iter->second ;
        return ;
      } // if
    } // for
  } // if

  if ( mCurrentCommand.mIsFunction ) {  // ��O���Ofunction���Ѽ�
    for ( int i = 0 ; i < mCurrentCommand.mParameterList.size() ; i++ ) {
      if ( mCurrentCommand.mParameterList[i].name == id ) {
        varInfo = mCurrentCommand.mParameterList[i] ;
        return ;
      } // if
    } // for

    // �O���Ofunction�w�q���Ѽ�
    for ( int i = mCurrentCommand.mFuncDefineVar.size() - 1 ; i >= 0 ; i-- ) {
      iter = mCurrentCommand.mFuncDefineVar[i].find( id ) ;
      if ( iter != mCurrentCommand.mFuncDefineVar[i].end() ) {
        varInfo = iter->second ;
        return ;
      } // if
    } // for

    throw new UndefinedIdError( id, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new UndefinedIdError( id, mCommandLine, mNextChar ) ;
} // C_Interpreter::IsIdDefined()

void C_Interpreter :: IsFunctionDefined( string functionName ) {

  map<string, Function>::iterator iter ;
  iter = mFunctionTable.find( functionName ) ;
  if ( iter != mFunctionTable.end() ) { // �Ofunction�W��
    return ;
  } // if
  else if ( functionName == mCurrentCommand.mfuncName ) { // ���j
    return ;
  } // else if
  else
    throw new UndefinedIdError( functionName, mCommandLine, mNextChar ) ;

} // C_Interpreter::IsFunctionDefined()

// /************************************* Syntactical Analysis********************************************///

// Done() ; | ListVariable() ; | ListFunction() ; | ListAllVariables() ; | ListAllFunctions() ;
// user_input : ( definition | statement ) { definition | statement }
void C_Interpreter :: User_Input( bool & isDone ) {

  TokenType type ;
  string tokenStr ;
  PeekToken( type, tokenStr ) ;
  bool hasComment ;


  if ( tokenStr == "Done" || tokenStr == "ListVariable" || tokenStr == "ListFunction" ||
       tokenStr == "ListAllVariables" || tokenStr == "ListAllFunctions" ) {
    mCurrentCommand.mIsSystemFunction = true ;
    SystemSupportFunction( isDone ) ;
  } // if
  else if ( type == INPUTBUFFERAPPEND ) {  // project3�s�W
    InputBufferAppend() ;
    cout << "Input-buffer appended ..." << endl ;
  } // else if
  else { // ( definition | statement ) { definition | statement }

    if ( type == VOID || Type_specifier( type ) ) { // Definition
      Definition() ;
    } // if
    else { // Statement
      mCurrentCommand.mIsStatement = true ;
      Statement() ;
    } // else if
  } // else if

} // C_Interpreter::User_Input()

void C_Interpreter :: SystemSupportFunction( bool & isDone ) {


  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ;
  // get "Done" or "ListVariable" or "ListFunction" or "ListAllVariables" or "ListAllFunctions"

  if ( tokenStr == "Done" ) {
    Done() ;
    isDone = true ;
    return ;
  } // if
  else if ( tokenStr == "ListVariable" ) {
    string variableName ;
    ListVariableOrFunction( variableName ) ;
    ListVariable( variableName ) ;
    cout << "Statement executed ..." << endl ;
  } // else if
  else if ( tokenStr == "ListFunction" ) {
    string functionName ;
    ListVariableOrFunction( functionName ) ;
    ListFunction( functionName )  ;
    cout << "Statement executed ..." << endl ;
  } // else if
  else if ( tokenStr == "ListAllVariables" ) {
    ListAllVariablesOrFunctions() ;

    for ( map<string, Variable>::iterator it = mVarTable[0].begin() ; it != mVarTable[0].end() ; it++ ) {
      cout << ( *it ).first << endl ;
    } // for

    cout << "Statement executed ..." << endl ;
  } // else if
  else if ( tokenStr == "ListAllFunctions" ) {
    ListAllVariablesOrFunctions() ;
    for ( map<string, Function>::iterator it = mFunctionTable.begin() ; it != mFunctionTable.end() ; it++ ) {
      cout << ( *it ).first << "()" << endl ;
    } // for

    cout << "Statement executed ..." << endl ;
  } // else if

} // C_Interpreter::SystemSupportFunction()


void C_Interpreter :: Done() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  // GetToken( type, tokenStr, false, hasComment ) ; // get "Done"
  GetToken( type, tokenStr, false, hasComment ) ; // want "("
  if ( type == LEFT_SAMLL_PAREN ) {
    GetToken( type, tokenStr, false, hasComment ) ; //  want ")"
    if ( type == RIGHT_SAMLL_PAREN ) {
      Semicolon() ;
    } // if
    else {
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
    } // else if
  } // if
  else {
    throw new UndefinedIdError( tokenStr, mCommandLine, mNextChar ) ;
  } // else
} // C_Interpreter::Done()

void C_Interpreter :: ListVariableOrFunction( string & identStr ) {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  // GetToken( type, tokenStr, false, hasComment ) ; // get "ListVariable" or "ListFunction"
  GetToken( type, tokenStr, false, hasComment ) ;

  if ( type == LEFT_SAMLL_PAREN ) {
    GetToken( type, tokenStr, false, hasComment ) ; // want CONSTANT begin at '"'
    if ( type == CONSTANT && tokenStr[0] == '"' ) { // �n�O���޸��}�Y��CONSTSNT�~�ŦX�W�w
      identStr = tokenStr.substr( 1, tokenStr.size()-2 ); // �����Y�������޸��A���o�n���ܼƦW�١A���Y�O""
      GetToken( type, tokenStr, false, hasComment ) ; // want ")"
      if ( type == RIGHT_SAMLL_PAREN ) {
        Semicolon() ;
      } // if
      else {
        throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
      } // else
    } // if
    else {
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
    } // else if
  } // if
  else {
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else
} // C_Interpreter::ListVariableOrFunction()

void C_Interpreter :: ListVariable( string variableName ) {

  map<string, Variable>::iterator iter ;
  iter = mVarTable[0].find( variableName ) ;
  if ( iter != mVarTable[0].end() ) { // ���o���ܼ�
    if ( iter->second.isArray ) { // �o���ܼƬO�}�C
      cout << iter->second.type << " " << iter->second.name
            << "[ " << iter->second.arraySize <<  " ] ;" << endl ;
    } // if
    else {
      cout << iter->second.type << " " << iter->second.name << " ;" << endl ;
    } // else if
  } // if
  else { // �S���o���ܼƦW��
    throw new UndefinedIdError( variableName, mCommandLine, mNextChar ) ;
  } // else
} // C_Interpreter::ListVariable()

void C_Interpreter :: ListFunction( string functionName ) {

  map<string, Function>::iterator iter ;
  iter = mFunctionTable.find( functionName ) ;
  if ( iter != mFunctionTable.end() ) { // ���o��function
    int level = 0 ;
    int i = 0 ;
    bool isStart = true ;
    while ( i < iter->second.tokenList.size() ) {
      PrintFunction( i, iter, level, isStart ) ;
    } // while

  } // if
  else { // �S���o��function
    throw new UndefinedIdError( functionName, mCommandLine, mNextChar ) ;
  } // else
} // C_Interpreter::ListFunction()

void C_Interpreter :: PrintFunction( int & i, map<string, Function>::iterator & iter,
                                     int & level, bool & isStart ) {

  TokenType tt = iter->second.tokenList[i].type ;
  string tokenStr = iter->second.tokenList[i].tokenStr ;

  if ( tt == RIGHT_BIG_PAREN ) // �Y�J��S�j�A���A�h�h�ƴ�1
    level-- ;

  if ( isStart ) // �n�L�s���@��e�����ť�
    PrintSpace( level ) ;

  isStart = false ;
  if ( tt == VOID || Type_specifier( tt ) ) {
    cout << tokenStr << " " ;
    i++ ;
  } // if
  else if ( tt == IDENT ) {
    cout << tokenStr ;
    TokenType nextType = iter->second.tokenList[ i+1 ].type ;
    if ( nextType != LEFT_MID_PAREN && nextType != LEFT_SAMLL_PAREN && nextType != PP &&
         nextType != MM && nextType != COMMA )
      cout << " " ;
    i++ ;
  } // else if
  else if ( tt == PP || tt == MM ) {
    cout << tokenStr ;
    TokenType nextType = iter->second.tokenList[ i+1 ].type ;
    if ( nextType != IDENT )
      cout << " " ;
    i++ ;
  } // else if
  else if ( tt == SIGN ) {
    cout << tokenStr ;
    i++ ;
  } // else if
  else if ( tt == LEFT_SAMLL_PAREN ) {
    cout << tokenStr ;
    TokenType nextType = iter->second.tokenList[ i+1 ].type ;
    if ( nextType != RIGHT_SAMLL_PAREN )
      cout << " " ;
    i++ ;
  } // else if
  else if ( tt == LEFT_MID_PAREN || tt == RIGHT_SAMLL_PAREN ) {
    cout << tokenStr << " " ;
    i++ ;
  } // else if
  else if ( tt == RIGHT_MID_PAREN ) {
    cout << tokenStr ;
    TokenType nextType = iter->second.tokenList[ i+1 ].type ;
    if ( nextType != COMMA )
      cout << " " ;
    i++ ;
  } // else if
  else if ( tt == LEFT_BIG_PAREN ) { // �J�쥪�j�A���A�L���n����B�h��+1
    cout << tokenStr << endl ;
    isStart = true ;
    level++ ;
    i++ ;
  } // else if
  else if ( tt == RIGHT_BIG_PAREN || tt == SEMICOLON ) { // �J��k�j�A���Τ����A�L���n����
    cout << tokenStr << endl ;
    isStart = true ;
    i++ ;
  } // else if
  else if ( tt == IF ) {
    PrintIfOrWhile( i, iter, level, isStart ) ;
  } // else if
  else if ( tt == ELSE ) {
    cout << tokenStr << " " ; // print "else"
    i++ ;
    PrintStatement( i, iter, level, isStart ) ;
  } // else if
  else if ( tt == WHILE ) {
    PrintIfOrWhile( i, iter, level, isStart ) ;
  } // else if
  else if ( tt == DO ) {
    cout << tokenStr << " " ; // print "do"
    i++ ;
    PrintStatement( i, iter, level, isStart ) ;

    if ( iter->second.tokenList[i].type == LEFT_BIG_PAREN ) {
      int leftParenNum = 0 ;
      do {
        TokenType next = iter->second.tokenList[i].type ;
        if ( next == LEFT_BIG_PAREN )
          leftParenNum++ ;
        else if ( next == RIGHT_BIG_PAREN )
          leftParenNum-- ;
        PrintFunction( i, iter, level, isStart ) ;
      } while ( i < iter->second.tokenList.size() && leftParenNum > 0 ) ;
    } // if

    PrintSpace( level ) ;
    isStart = false ;
    cout << iter->second.tokenList[i].tokenStr << " " ; // print "while"
    i++ ;
    PrintParenExpression( i, iter, level, isStart ) ;
  } // else if
  else {
    cout << tokenStr << " " ;
    i++ ;
  } // else if

} // C_Interpreter::PrintFunction()

void C_Interpreter :: PrintParenExpression( int & i, map<string, Function>::iterator & iter,
                                            int & level, bool & isStart ) {

  int leftParenNum = 0 ;
  do { // �L��if while�����󵲧�
    TokenType next = iter->second.tokenList[i].type ;
    if ( next == LEFT_SAMLL_PAREN )
      leftParenNum++ ;
    else if ( next == RIGHT_SAMLL_PAREN )
      leftParenNum-- ;

    PrintFunction( i, iter, level, isStart ) ;
  } while ( i < iter->second.tokenList.size() && leftParenNum > 0 ) ;


} // C_Interpreter::PrintParenExpression()

void C_Interpreter :: PrintIfOrWhile( int & i, map<string, Function>::iterator & iter,
                                      int & level, bool & isStart ) {

  cout << iter->second.tokenList[i].tokenStr << " " ; // print if or while
  i++ ; // (
  PrintParenExpression( i, iter, level, isStart ) ;
  PrintStatement( i, iter, level, isStart ) ;

} // C_Interpreter::PrintIfOrWhile()

void C_Interpreter :: PrintStatement( int & i, map<string, Function>::iterator & iter,
                                      int & level, bool & isStart ) {

  TokenType next = iter->second.tokenList[i].type ;
  if ( next != LEFT_BIG_PAREN ) { // �ݬO�_���j�A��
    cout << endl ;
    level++ ;
    isStart = true ;
    while ( i < iter->second.tokenList.size() && next != SEMICOLON  ) {
      PrintFunction( i, iter, level, isStart ) ;
      next = iter->second.tokenList[i].type ;
    } // while

    cout << iter->second.tokenList[i].tokenStr << endl ; // print ";"
    isStart = true ;
    level-- ;
    i++ ;
  } // if

} // C_Interpreter::PrintStatement()

void C_Interpreter :: PrintSpace( int level ) {
  for ( int i = 0 ; i < level * 2 ; i++ ) {
    cout << " " ;
  } // for
} // C_Interpreter::PrintSpace()

void C_Interpreter :: ListAllVariablesOrFunctions() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  // GetToken( type, tokenStr, false, hasComment ) ; // get "ListAllVariables" or "ListAllFunctions"
  GetToken( type, tokenStr, false, hasComment ) ; // want "("

  if ( type == LEFT_SAMLL_PAREN ) {
    GetToken( type, tokenStr, false, hasComment ) ; // want ")"
    if ( type == RIGHT_SAMLL_PAREN ) {
      Semicolon() ;
    } // if
    else {
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
    } // else if
  } // if
  else {
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else
} // C_Interpreter::ListAllVariablesOrFunctions()

void C_Interpreter :: StoreFunction( string returnType, string identName ) {

  map<string, Function>::iterator iter ;
  iter = mFunctionTable.find( identName ) ;
  vector<Token> funcBody ;
  int i = 0 ;
  for ( ; i < mCurrentCommand.mTokenList.size() && mCurrentCommand.mTokenList[i].type != LEFT_BIG_PAREN ;
        i++ ) ;
  funcBody.assign( mCurrentCommand.mTokenList.begin() + i, mCurrentCommand.mTokenList.end() ) ;

  if ( iter != mFunctionTable.end() ) { // ���Ʃw�q
    iter->second.tokenList = mCurrentCommand.mTokenList ;
    iter->second.parameterList = mCurrentCommand.mParameterList ;
    iter->second.returnType = returnType ;
    iter->second.body = funcBody ;
    iter->second.level = 0 ;
    iter->second.funcDefineVar.clear() ;
    cout << "New definition of " << identName << "() entered ..." << endl ;
  } // if
  else { // �s�w�q��
    pair<string, Function> aFunction ;
    aFunction.first = identName ;
    aFunction.second.name = identName ;
    aFunction.second.parameterList = mCurrentCommand.mParameterList ;
    aFunction.second.tokenList = mCurrentCommand.mTokenList ;
    aFunction.second.returnType = returnType ;
    aFunction.second.body = funcBody ;
    aFunction.second.level = 0 ;
    mFunctionTable.insert( aFunction ) ;
    cout << "Definition of " << identName << "() entered ..." << endl ;
  } // else

} // C_Interpreter::StoreFunction()

void C_Interpreter :: StoreVariable( Variable var ) {  // �s�����ܼ�

  string varName = var.name ;
  map<string, Variable>::iterator iter ;
  // mCurrentCommand.identName = varName ;
  bool firstVar = false ;
  if ( mVarTable.size() == 0 )
    firstVar = true ;
  else
    iter = mVarTable[0].find( varName ) ;

  if ( ! firstVar && iter != mVarTable[0].end() ) { // ���Ʃw�q
    iter->second = var ;
    cout << "New definition of " << varName << " entered ..." << endl ;
  } // if
  else { // �s�w�q��
    pair<string, Variable> aVariable ;
    aVariable.first = varName ;
    aVariable.second = var ;
    if ( firstVar ) {
      map<string, Variable> newLevel ;
      newLevel.insert( aVariable ) ;
      mVarTable.push_back( newLevel ) ;
    } // if
    else
      mVarTable[0].insert( aVariable ) ;
    cout << "Definition of " << varName << " entered ..." << endl ;
  } // else

} // C_Interpreter::StoreVariable()


// definition : VOID Identifier function_definition_without_ID
//           | type_specifier Identifier function_definition_or_declarators
void C_Interpreter :: Definition() {
  // �I�sDefinition()�e�|�T�w�U�@��token�@�w�Ovoid��type_specifier
  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  // get "void" or "int" or "char" or "float" or "string" or "bool"
  GetToken( type, tokenStr, false, hasComment ) ;
  if ( type == VOID ) { // VOID Identifier function_definition_without_ID �A void �}�Y�@�w�O�w�qfunction
    mCurrentCommand.mIsFunction = true ;
    string returnType = tokenStr ; // function�^�ǫ��O
    GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
    if ( type == IDENT ) {
      string identName = tokenStr ;
      mCurrentCommand.mfuncName = tokenStr ;
      Function_definition_without_ID() ;
      StoreFunction( returnType, identName ) ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else { // type_specifier Identifier function_definition_or_declarators

    string identType = tokenStr ;
    GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
    if ( type == IDENT ) {
      string identName = tokenStr ;
      mCurrentCommand.mfuncName = tokenStr ;
      Function_definition_or_declarators( identType, identName ) ;
      if ( mCurrentCommand.mIsFunction ) {
        StoreFunction( identType, identName ) ;
      } // if
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

  } // else if

} // C_Interpreter::Definition()

// type_specifier : INT | CHAR | FLOAT | STRING | BOOL
bool C_Interpreter :: Type_specifier( TokenType type ) {
  return ( type == INT || type == CHAR || type == FLOAT || type == STRING || type == BOOL ) ;
} // C_Interpreter::Type_specifier()


// function_definition_or_declarators
//    : function_definition_without_ID
//     | rest_of_declarators
void C_Interpreter :: Function_definition_or_declarators( string identType, string firstIdentName ) {
  TokenType type ;
  string tokenStr ;
  PeekToken( type, tokenStr ) ;
  if ( type == LEFT_SAMLL_PAREN ) { // function_definition_without_ID �H "(" ���}�Y
    mCurrentCommand.mIsFunction = true ;
    Function_definition_without_ID() ;
  } // if
  else { // rest_of_declarators  �ܼƳѾl�ŧi������
    mCurrentCommand.mIsDeclaration = true ;
    Rest_of_declarators( identType, firstIdentName ) ;
  } // else if

} // C_Interpreter::Function_definition_or_declarators()

// rest_of_declarators
//     : [ '[' Constant ']' ]
//       { ',' Identifier [ '[' Constant ']' ] } ';'
void C_Interpreter :: Rest_of_declarators( string identType, string firstIdentName ) {
  // �Ѿl�ܼƫŧi�������A�e�������O���O+�ܼƦW��
  // �����ܼƷ|�s��mVariableTable�B��0�h��mVarList��(mVarList[0])�A�ϰ��ܼƦs��Ӽh��mVarList
  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  vector<Variable> store ; // ���O���Ҧ��ŧi���ܼơA�̫�T�w��k���T�A�s�_��
  Variable var ; // �s�ܼ�
  var.name = firstIdentName ;
  var.level = mLevel ;
  var.type = identType ;
  var.referenceVar = "" ;
  Optional_MidParen_Constant( var.isArray, var.arraySize ) ;
  InitializeVariable( var ) ; // ��l���ܼƭ�
  store.push_back( var ) ;
  bool repeat = true ;


  do {  // ���u�@���ܼƫŧi
    Variable newVar ; // �s�ܼ�
    newVar.type = identType ;
    newVar.level = mLevel ;
    newVar.referenceVar = "" ;
    PeekToken( type, tokenStr ) ; // peek whether is ","
    if ( type == COMMA ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get ","
      GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
      if ( type == IDENT ) {
        newVar.name = tokenStr ;
        Optional_MidParen_Constant( newVar.isArray, newVar.arraySize ) ;
        InitializeVariable( newVar ) ; // ��l���ܼƭ�
        store.push_back( newVar ) ;
      } // if
      else
        throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
    } // if
    else
      repeat = false ;

  } while ( repeat ) ;

  Semicolon() ;

  if ( ! mCurrentCommand.mIsFunction ) {
    if ( mLevel == 0 ) { // �s�����ܼ�
      for ( int i = 0 ; i < store.size() ; i++ )
        StoreVariable( store[i] ) ;
    } // if
    else { // �x�s�ϰ��ܼ�
      StoreLocalVariable( store ) ;
    } // else
  } // if
  else { // �Ofunction�w�q���ܼ�
    StoreFunctionLocalVar( store ) ;
  } // else

  // �N�ŧi���ܼ��x�s�_��

  // PrintVar() ;

} // C_Interpreter::Rest_of_declarators()

void C_Interpreter :: StoreLocalVariable( vector<Variable> localVar ) {

  if ( mLevel >= mVarTable.size() ) { // mVarTable�S���{�b�h�ƪ��ܼƪ�
    while ( mLevel >= mVarTable.size() ) { //
      map<string, Variable> newLevel ;
      mVarTable.push_back( newLevel ) ;
    } // while
  } // if

  for ( int i = 0 ; i < localVar.size() ; i++ ) {

    map<string, Variable>::iterator iter ;
    iter = mVarTable[mLevel].find( localVar[i].name ) ;
    if ( iter != mVarTable[mLevel].end() ) { // �o�@�h���@�˦W�٪��ܼ�
      iter->second = localVar[i] ;
    } // if
    else {
      pair<string, Variable> aVariable ;
      aVariable.first = localVar[i].name ;
      aVariable.second = localVar[i] ;
      mVarTable[mLevel].insert( aVariable ) ;
    } // else
  } // for

} // C_Interpreter::StoreLocalVariable()


void C_Interpreter :: StoreFunctionLocalVar( vector<Variable> funcVar ) {

  if ( mCurrentCommand.mFuncLevel >= mCurrentCommand.mFuncDefineVar.size() ) { // mVarTable�S���{�b�h�ƪ��ܼƪ�
    while ( mCurrentCommand.mFuncLevel >= mCurrentCommand.mFuncDefineVar.size() ) { //
      map<string, Variable> newLevel ;
      mCurrentCommand.mFuncDefineVar.push_back( newLevel ) ;
    } // while
  } // if

  for ( int i = 0 ; i < funcVar.size() ; i++ ) {
    map<string, Variable>::iterator iter ;
    iter = mCurrentCommand.mFuncDefineVar[mCurrentCommand.mFuncLevel].find( funcVar[i].name ) ;
    if ( iter != mCurrentCommand.mFuncDefineVar[mCurrentCommand.mFuncLevel].end() ) { // �o�@�h���@�˦W�٪��ܼ�
      iter->second = funcVar[i] ;
    } // if
    else {
      pair<string, Variable> aVariable ;
      aVariable.first = funcVar[i].name ;
      aVariable.second = funcVar[i] ;
      mCurrentCommand.mFuncDefineVar[mCurrentCommand.mFuncLevel].insert( aVariable ) ;
    } // else
  } // for
} // C_Interpreter::StoreFunctionLocalVar()

// function_definition_without_ID
//     : '(' [ VOID | formal_parameter_list ] ')' compound_statement

void C_Interpreter :: Function_definition_without_ID() {

  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  GetToken( type, tokenStr, false, hasComment ) ; // want "("
  if ( type == LEFT_SAMLL_PAREN ) {
    PeekToken( type, tokenStr ) ; // peek whether is void or formal_parameter_list
    if ( type == VOID ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "void"
    } // if
    else if ( Type_specifier( type ) ) { // formal_parameter_list �O�H type_specifier ���}�Y
      Formal_parameter_list() ; // function���ѼƷ|�s�bmCurrentCommand.mParameterList
    } // else if

    GetToken( type, tokenStr, false, hasComment ) ; // want ")"
    if ( type == RIGHT_SAMLL_PAREN ) {
      Compound_statement() ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

} // C_Interpreter::Function_definition_without_ID()

// formal_parameter_list
//     : type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
//       { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }

void C_Interpreter :: Formal_parameter_list() {  // function���ѼƦC�A�ѼƸ�T�s�bmmCurrentCommand.mParameterList
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;


  do {
    Variable aParameter ;
    // pair< string, Variable > aPara ; // string�ܼƦW�١AVariable�s�ܼƸ�T

    GetToken( type, tokenStr, false, hasComment ) ;  // want type_specifier
    if ( Type_specifier( type ) ) {
      aParameter.type = tokenStr ; // �ܼƫ��O
      PeekToken( type, tokenStr ) ; // want "&" or IDENT
      if ( type == ONE_AND ) {
        GetToken( type, tokenStr, false, hasComment ) ; // get "&"
        aParameter.passByReference = true ;
      } // if
      else
        aParameter.passByReference = false ;

      // aParameter.referenceVar = NULL ;
      GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
      if ( type == IDENT ) {
        // aPara.first = tokenStr ; // �ܼƦW��
        aParameter.name = tokenStr ;
        Optional_MidParen_Constant( aParameter.isArray, aParameter.arraySize ) ;
        mCurrentCommand.mParameterList.push_back( aParameter ) ;

        PeekToken( type, tokenStr ) ; // peek whether is ","
        if ( type == COMMA ) {
          GetToken( type, tokenStr, false, hasComment ) ;  // get ","
          repeat = true ;
        } // if
        else
          repeat = false ;
      } // if
      else
        throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

    } // if
    else {
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
    } // else if

  } while ( repeat ) ;


} // C_Interpreter::Formal_parameter_list()


// compound_statement
//     : '{' { declaration | statement } '}'
void C_Interpreter :: Compound_statement() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ; // want "{"
  if ( type == LEFT_BIG_PAREN ) {
    if ( ! mCurrentCommand.mIsFunction )
      mLevel++ ;  // �J��@�ӥ��j�A���h�ƴN�[1
    else
      mCurrentCommand.mFuncLevel++ ;
    bool repeat = true ;
    do {
      PeekToken( type, tokenStr ) ;
      if ( Type_specifier( type ) ) { // declaration �H type_specifier ���}�Y
        Declaration() ;  // �ܼƫŧi������
      } // if
      else if ( type == SEMICOLON || IsBasicExpressionBeginToken( type ) || type == RETURN ||
                type == LEFT_BIG_PAREN || type == IF || type == WHILE || type == DO  ) { // statement���}�Y
        Statement() ;
      } // else if
      else {
        repeat = false ;
      } // else if

    } while ( repeat ) ;

    GetToken( type, tokenStr, false, hasComment ) ; // want "}"

    if ( type == RIGHT_BIG_PAREN ) { // �J��@�ӥk�j�A���h�ƴN��1


      if ( ! mCurrentCommand.mIsFunction ) {
        if ( mLevel == mVarTable.size() -1 ) { // �b�o�@�h���ŧi�ܼơA�h�b�k�j�A������N����ΤF
          mVarTable.pop_back() ;
        } // if

        mLevel-- ;
      } // if
      else {
        if ( mCurrentCommand.mFuncLevel == mCurrentCommand.mFuncDefineVar.size() -1 ) {
          // �b�o�@�h���ŧi�ܼơA�h�b�k�j�A������N����ΤF
          mCurrentCommand.mFuncDefineVar.pop_back() ;
        } // if

        mCurrentCommand.mFuncLevel-- ;
      } // else
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

} // C_Interpreter::Compound_statement()


// declaration
//     : type_specifier Identifier rest_of_declarators
void C_Interpreter :: Declaration() { // function�Bif while�̭����ŧi�ܼ�

  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ; // want type_specifier
  if ( Type_specifier( type ) ) {
    string identType = tokenStr ; // �ܼƫ��O
    GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
    if ( type == IDENT ) {
      string firstIdentName = tokenStr ; // �ܼƦW��
      Rest_of_declarators( identType, firstIdentName ) ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

} // C_Interpreter::Declaration()


// statement
//     : ';'     // the null statement
//     | expression ';'  /* expression here should not be empty */
//     | RETURN [ expression ] ';'
//     | compound_statement
//     | IF '(' expression ')' statement [ ELSE statement ]
//     | WHILE '(' expression ')' statement
//     | DO statement WHILE '(' expression ')' ';'

void C_Interpreter :: Statement() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ;
  if ( type == SEMICOLON ) { // ";"
    GetToken( type, tokenStr, false, hasComment ) ; // get ";"
    return ;
  } // if
  else if ( IsBasicExpressionBeginToken( type ) ) { // expression ';'
    // expression �i��H IDENT, PP, MM, sign, "(" ���}�Y
    Expression() ;
    Semicolon() ;
  } // else if
  else if ( type == RETURN ) { // RETURN [ expression ] ';'
    GetToken( type, tokenStr, false, hasComment ) ; // get RETURN
    PeekToken( type, tokenStr ) ; // peek whether is start of expression
    if ( IsBasicExpressionBeginToken( type ) ) {
      Expression() ;
    } // if

    Semicolon() ;
  } // else if
  else if ( type == LEFT_BIG_PAREN ) { // compound_statement �H "{" ���}�Y
    Compound_statement() ;
  } // else if
  else if ( type == IF ) { // IF '(' expression ')' statement [ ELSE statement ]
    GetToken( type, tokenStr, false, hasComment ) ; // get IF
    SmallParenExpression() ;
    Statement() ;
    PeekToken( type, tokenStr ) ; // peek whether is ELSE
    if ( type == ELSE ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get ELSE
      Statement() ;
    } // if
  } // else if
  else if ( type == WHILE ) { // WHILE '(' expression ')' statement
    GetToken( type, tokenStr, false, hasComment ) ; // get WHILE
    SmallParenExpression() ;
    Statement() ;
  } // else if
  else if ( type == DO ) { // DO statement WHILE '(' expression ')' ';'
    GetToken( type, tokenStr, false, hasComment ) ; // get DO
    Statement() ;
    GetToken( type, tokenStr, false, hasComment ) ; // want WHILE
    if ( type == WHILE ) {
      SmallParenExpression() ;
      Semicolon() ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if
  else {
    GetToken( type, tokenStr, false, hasComment ) ;
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if

} // C_Interpreter::Statement()

// basic_expression �i��H IDENT, PP, MM, sign, CONSTANT, "(" ���}�Y  �h�[CIN COUT
bool C_Interpreter :: IsBasicExpressionBeginToken( TokenType type ) {
  return ( type == IDENT || type == PP || type == MM || IsSign( type ) || type == CONSTANT ||
           type == LEFT_SAMLL_PAREN ||  type == CIN || type == COUT || type == SIGN ) ; //
} // C_Interpreter::IsBasicExpressionBeginToken()

// expression : basic_expression { ',' basic_expression }
void C_Interpreter :: Expression() {

  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  Basic_expression() ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is ","
    if ( type == COMMA ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get ","
      Basic_expression() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::Expression()

// basic_expression
//     : Identifier rest_of_Identifier_started_basic_exp
//     | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
//     | sign { sign } signed_unary_exp romce_and_romloe
//     | ( Constant | '(' expression ')' ) romce_and_romloe
// �i��n�h�[�� : CIN COUT
void C_Interpreter :: Basic_expression() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ;
  if ( type == IDENT ) { // Identifier rest_of_Identifier_started_basic_exp
    GetToken( type, tokenStr, false, hasComment ) ; // get IDENT
    string identName = tokenStr ;
    PeekToken( type, tokenStr ) ; // peek whether is "("
    if ( type == LEFT_SAMLL_PAREN ) { // �I�sfunction
      IsFunctionDefined( identName ) ;
    } // if
    else {
      Variable identInfo ;
      IsIdDefined( identName, identInfo ) ;
    } // end else

    Rest_of_Identifier_started_basic_exp() ;
  } // if
  else if ( type == PP || type == MM ) { // ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
    GetToken( type, tokenStr, false, hasComment ) ; // get PP or MM
    GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
    if ( type == IDENT ) {
      Variable identInfo ;
      IsIdDefined( tokenStr, identInfo ) ;
      Rest_of_PPMM_Identifier_started_basic_exp() ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if
  else if ( IsSign( type ) ) { // sign { sign } signed_unary_exp romce_and_romloe
    GetToken( type, tokenStr, false, hasComment ) ; // get SIGN
    mCurrentCommand.mTokenList[ mCurrentCommand.mTokenList.size()-1 ].type = SIGN ;
    bool repeat = true ;
    do {
      PeekToken( type, tokenStr ) ; // peek whether is SIGN
      if ( IsSign( type ) ) {
        GetToken( type, tokenStr, false, hasComment ) ; // get SIGN
        mCurrentCommand.mTokenList[ mCurrentCommand.mTokenList.size()-1 ].type = SIGN ;
      } // if
      else
        repeat = false ;
    } while ( repeat ) ;
    Signed_unary_exp() ;
    Romce_and_romloe() ;

  } // else if
  else if ( type == CONSTANT || type == LEFT_SAMLL_PAREN ) {
    // ( Constant | '(' expression ')' ) romce_and_romloe
    if ( type == CONSTANT ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get CONSTANT
    } // if
    else // "("
      SmallParenExpression() ;

    Romce_and_romloe() ;
  } // else if
  else if ( type == CIN ) {

    GetToken( type, tokenStr, false, hasComment ) ; // get CIN
    GetToken( type, tokenStr, false, hasComment ) ; // want RS
    if ( type == RS ) {

      bool repeat = true ;
      do {

        GetToken( type, tokenStr, false, hasComment ) ; // want IDENT

        if ( type == IDENT ) {

          string varName = tokenStr ;
          Variable varInfo ;
          IsIdDefined( varName, varInfo ) ; // cin�᭱�u�౵�ܼƦW�١AcheckFunction��false

          PeekToken( type, tokenStr ) ; // peek whether is "["  �P�_�O�_���}�C

          if ( type == LEFT_MID_PAREN ) { // �O�}�C
            MidParenExpression() ;
          } // if
          else { // ���O�}�C
            if ( varInfo.isArray && varInfo.type != "char" ) // ���o���ܼƥ��e�O�Q�ŧi���}�C
              throw new UndefinedIdError( varName, mCommandLine, mNextChar ) ;
          } // else

          PeekToken( type, tokenStr ) ; // peek whether is RS
          if ( type == RS ) {
            GetToken( type, tokenStr, false, hasComment ) ; // get RS
          } // if
          else
            repeat = false ;
        } // if
        else
          throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
      } while ( repeat ) ;

      // AssignValueToIdent( vars, index ) ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if
  else if ( type == COUT ) {
    GetToken( type, tokenStr, false, hasComment ) ; // get COUT
    GetToken( type, tokenStr, false, hasComment ) ; // want LS
    if ( type == LS ) {
      Expression() ;
      bool repeat = true ;
      do {
        PeekToken( type, tokenStr ) ; // peek whether is LS
        if ( type == LS ) {
          GetToken( type, tokenStr, false, hasComment ) ; // get LS
          Expression() ;
        } // if
        else
          repeat = false ;
      } while ( repeat ) ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if
  else {
    GetToken( type, tokenStr, false, hasComment ) ;
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else

} // C_Interpreter::Basic_expression()


// rest_of_Identifier_started_basic_exp
//     : [ '[' expression ']' ]
//       ( assignment_operator basic_expression
//         |
//         [ PP | MM ] romce_and_romloe
//       )
//     | '(' [ actual_parameter_list ] ')' romce_and_romloe
void C_Interpreter :: Rest_of_Identifier_started_basic_exp() {
  // cout << "Rest_of_Identifier_started_basic_exp" << endl ;
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ; // peek whether is "("
  if ( type == LEFT_SAMLL_PAREN ) { //  '(' [ actual_parameter_list ] ')' romce_and_romloe
    SmallParenOptionalActual_parameter_list() ;
    Romce_and_romloe() ;
  } // if
  else { // [ '[' expression ']' ]  ( assignment_operator basic_expression  |  [ PP | MM ] romce_and_romloe )
    if ( type == LEFT_MID_PAREN ) {
      MidParenExpression() ;
    } // if

    PeekToken( type, tokenStr ) ; // peek whether is assignment_operator
    if ( Assignment_operator( type ) ) { // assignment_operator basic_expression
      GetToken( type, tokenStr, false, hasComment ) ; // get assignment_operator
      // cout << TokenTypeToString( type ) << "  " ;
      Basic_expression() ;
    } // if
    else {  // [ PP | MM ] romce_and_romloe
      if ( type == PP || type == MM ) {
        GetToken( type, tokenStr, false, hasComment ) ; // get PP or MM
      } // if

      Romce_and_romloe() ;
    } // else if

  } // else if


} // C_Interpreter::Rest_of_Identifier_started_basic_exp()


// rest_of_PPMM_Identifier_started_basic_exp
//     : [ '[' expression ']' ] romce_and_romloe
void C_Interpreter :: Rest_of_PPMM_Identifier_started_basic_exp() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ;
  if ( type == LEFT_MID_PAREN ) {
    MidParenExpression() ;
  } // if

  Romce_and_romloe() ;

} // C_Interpreter::Rest_of_PPMM_Identifier_started_basic_exp()

// sign : '+' | '-' | '!'
bool C_Interpreter :: IsSign( TokenType type ) {
  return ( type == PLUS || type == MINUS || type == NOT ) ;
} // C_Interpreter::IsSign()


// actual_parameter_list
//     : basic_expression { ',' basic_expression }
void C_Interpreter :: Actual_parameter_list() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  Basic_expression() ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is ","
    if ( type == COMMA ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get ","
      Basic_expression() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::Actual_parameter_list()

// assignment_operator : '=' | TE | DE | RE | PE | ME
bool C_Interpreter :: Assignment_operator( TokenType type ) { // = *= /= %= /= -=
  return ( type == ASSIGN || type == TE || type == DE || type == RE || type == PE || type == ME ) ;
} // C_Interpreter::Assignment_operator()

// rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp // �Yromce_and_romloe
//     : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
void C_Interpreter :: Romce_and_romloe() {

  // cout << "Romce_and_romloe ######" << endl ;
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  Rest_of_maybe_logical_OR_exp() ;
  PeekToken( type, tokenStr ) ; // peek whether is "?"
  if ( type == QUES ) {
    GetToken( type, tokenStr, false, hasComment ) ; // get "?"
    Basic_expression() ;
    GetToken( type, tokenStr, false, hasComment ) ; // wnat ":"
    if ( type == COLON ) {
      Basic_expression() ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

  } // if

} // C_Interpreter::Romce_and_romloe()


// rest_of_maybe_logical_OR_exp
//     : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
void C_Interpreter :: Rest_of_maybe_logical_OR_exp() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  Rest_of_maybe_logical_AND_exp() ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is OR
    if ( type == OR ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get OR
      Maybe_logical_AND_exp() ;
    } // if
    else
      repeat = false ;

  } while ( repeat ) ;

} // C_Interpreter::Rest_of_maybe_logical_OR_exp()

// maybe_logical_AND_exp
//     : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
void C_Interpreter :: Maybe_logical_AND_exp() {

  Maybe_bit_OR_exp() ;
  AND_Maybe_bit_OR_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_logical_AND_exp()

// rest_of_maybe_logical_AND_exp
//     : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
void C_Interpreter :: Rest_of_maybe_logical_AND_exp() {

  Rest_of_maybe_bit_OR_exp() ;
  AND_Maybe_bit_OR_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_logical_AND_exp()

// { AND maybe_bit_OR_exp }
void C_Interpreter :: AND_Maybe_bit_OR_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is AND
    if ( type == AND ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get AND
      Maybe_bit_OR_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::AND_Maybe_bit_OR_exp_RepeatZeroToMany()

// maybe_bit_OR_exp
//     : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
void C_Interpreter :: Maybe_bit_OR_exp() {

  Maybe_bit_ex_OR_exp() ;
  ONEOR_Maybe_bit_ex_OR_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_bit_OR_exp()

// rest_of_maybe_bit_OR_exp
//     : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
void C_Interpreter :: Rest_of_maybe_bit_OR_exp() {

  Rest_of_maybe_bit_ex_OR_exp() ;
  ONEOR_Maybe_bit_ex_OR_exp_RepeatZeroToMany() ;


} // C_Interpreter::Rest_of_maybe_bit_OR_exp()

// { '|' maybe_bit_ex_OR_exp }
void C_Interpreter :: ONEOR_Maybe_bit_ex_OR_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is "|"
    if ( type == ONE_OR ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "|"
      Maybe_bit_ex_OR_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;
} // C_Interpreter::ONEOR_Maybe_bit_ex_OR_exp_RepeatZeroToMany()

// maybe_bit_ex_OR_exp
//     : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
void C_Interpreter :: Maybe_bit_ex_OR_exp() {

  Maybe_bit_AND_exp() ;
  CARET_Maybe_bit_AND_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_bit_ex_OR_exp()

// rest_of_maybe_bit_ex_OR_exp
//     : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
void C_Interpreter :: Rest_of_maybe_bit_ex_OR_exp() {

  Rest_of_maybe_bit_AND_exp() ;
  CARET_Maybe_bit_AND_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_bit_ex_OR_exp()

// { '^' maybe_bit_AND_exp }
void C_Interpreter :: CARET_Maybe_bit_AND_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is "^"
    if ( type == CARET ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "^"
      Maybe_bit_AND_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::CARET_Maybe_bit_AND_exp_RepeatZeroToMany()

// maybe_bit_AND_exp
//     : maybe_equality_exp { '&' maybe_equality_exp }
void C_Interpreter :: Maybe_bit_AND_exp() {

  Maybe_equality_exp() ;
  ONEAND_Maybe_equality_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_bit_AND_exp()

// rest_of_maybe_bit_AND_exp
//     : rest_of_maybe_equality_exp { '&' maybe_equality_exp }

void C_Interpreter :: Rest_of_maybe_bit_AND_exp() {

  Rest_of_maybe_equality_exp() ;
  ONEAND_Maybe_equality_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_bit_AND_exp()

// { '&' maybe_equality_exp }
void C_Interpreter :: ONEAND_Maybe_equality_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is "&"
    if ( type == ONE_AND ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "&"
      Maybe_equality_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::ONEAND_Maybe_equality_exp_RepeatZeroToMany()

// maybe_equality_exp
//     : maybe_relational_exp
//       { ( EQ | NEQ ) maybe_relational_exp}
void C_Interpreter :: Maybe_equality_exp() {

  Maybe_relational_exp() ;
  EQorNEQ_Maybe_relational_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_equality_exp()

// rest_of_maybe_equality_exp
//     : rest_of_maybe_relational_exp
//       { ( EQ | NEQ ) maybe_relational_exp }
void C_Interpreter :: Rest_of_maybe_equality_exp() {

  Rest_of_maybe_relational_exp() ;
  EQorNEQ_Maybe_relational_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_equality_exp()

// { ( EQ | NEQ ) maybe_relational_exp }
void C_Interpreter :: EQorNEQ_Maybe_relational_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is EQ or NEQ
    if ( type == EQUAL || type == NOTEQUAL ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get EQ or NEQ
      Maybe_relational_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::EQorNEQ_Maybe_relational_exp_RepeatZeroToMany()

// maybe_relational_exp
//     : maybe_shift_exp
//       { ( '<' | '>' | LE | GE ) maybe_shift_exp }
void C_Interpreter :: Maybe_relational_exp() {

  Maybe_shift_exp() ;
  LorGorLEorGE_Maybe_shift_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_relational_exp()


// rest_of_maybe_relational_exp
//     : rest_of_maybe_shift_exp
//       { ( '<' | '>' | LE | GE ) maybe_shift_exp }
void C_Interpreter :: Rest_of_maybe_relational_exp() {

  Rest_of_maybe_shift_exp() ;
  LorGorLEorGE_Maybe_shift_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_relational_exp()

// { ( '<' | '>' | LE | GE ) maybe_shift_exp }
void C_Interpreter :: LorGorLEorGE_Maybe_shift_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is "<" or ">" or LE or GE
    if ( type == LESS || type == GREATER || type == LE || type == GE ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "<" or ">" or LE or GE
      Maybe_shift_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::LorGorLEorGE_Maybe_shift_exp_RepeatZeroToMany()

// maybe_shift_exp
//     : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
void C_Interpreter :: Maybe_shift_exp() {

  Maybe_additive_exp() ;
  LSorRS_Maybe_additive_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_shift_exp()


// rest_of_maybe_shift_exp
//     : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
void C_Interpreter :: Rest_of_maybe_shift_exp() {

  Rest_of_maybe_additive_exp() ;
  LSorRS_Maybe_additive_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_shift_exp()

// { ( LS | RS ) maybe_additive_exp }
void C_Interpreter :: LSorRS_Maybe_additive_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;

  do {
    PeekToken( type, tokenStr ) ; // peek whether is LS or RS
    if ( type == LS || type == RS ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get LS or RS
      Maybe_additive_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::LSorRS_Maybe_additive_exp_RepeatZeroToMany()


// maybe_additive_exp
//     : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
void C_Interpreter :: Maybe_additive_exp() {

  Maybe_mult_exp() ;
  PLUSorMINUS_Maybe_mult_exp_RepeatZeroToMany() ;

} // C_Interpreter::Maybe_additive_exp()

// rest_of_maybe_additive_exp
//     : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
void C_Interpreter :: Rest_of_maybe_additive_exp() {

  Rest_of_maybe_mult_exp() ;
  PLUSorMINUS_Maybe_mult_exp_RepeatZeroToMany() ;

} // C_Interpreter::Rest_of_maybe_additive_exp()

// { ( '+' | '-' ) maybe_mult_exp }
void C_Interpreter :: PLUSorMINUS_Maybe_mult_exp_RepeatZeroToMany() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;

  do {
    PeekToken( type, tokenStr ) ; // peek whether is "+" or "-"
    if ( type == PLUS || type == MINUS ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "+" or "-"
      Maybe_mult_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::PLUSorMINUS_Maybe_mult_exp_RepeatZeroToMany()

// maybe_mult_exp
//     : unary_exp rest_of_maybe_mult_exp
void C_Interpreter :: Maybe_mult_exp() {
  Unary_exp() ;
  Rest_of_maybe_mult_exp() ;
} // C_Interpreter::Maybe_mult_exp()


// rest_of_maybe_mult_exp
//     : { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */
void C_Interpreter :: Rest_of_maybe_mult_exp() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  bool repeat = true ;
  do {
    PeekToken( type, tokenStr ) ; // peek whether is "*" or "/" or "%"
    if ( type == MULTIPLICAION || type == DIVISION || type == PERCENT ) {
      GetToken( type, tokenStr, false, hasComment ) ; // get "*" or "/" or "%"
      Unary_exp() ;
    } // if
    else
      repeat = false ;
  } while ( repeat ) ;

} // C_Interpreter::Rest_of_maybe_mult_exp()


// unary_exp
//     : sign { sign } signed_unary_exp
//     | unsigned_unary_exp
//     | ( PP | MM ) Identifier [ '[' expression ']' ]
void C_Interpreter :: Unary_exp() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ;
  if ( IsSign( type ) ) { // sign { sign } signed_unary_exp
    GetToken( type, tokenStr, false, hasComment ) ; // get SIGN
    mCurrentCommand.mTokenList[ mCurrentCommand.mTokenList.size()-1 ].type = SIGN ;
    bool repeat = true ;
    do {
      PeekToken( type, tokenStr ) ; // peek whether is SIGN
      if ( IsSign( type ) ) {
        GetToken( type, tokenStr, false, hasComment ) ; // get SIGN
        mCurrentCommand.mTokenList[ mCurrentCommand.mTokenList.size()-1 ].type = SIGN ;
      } // if
      else
        repeat = false ;
    } while ( repeat ) ;
    Signed_unary_exp() ;
  } // if
  else if ( type == IDENT || type == CONSTANT || type == LEFT_SAMLL_PAREN ) { // unsigned_unary_exp���}�Y
    Unsigned_unary_exp() ;
  } // else if
  else if ( type == PP || type == MM ) { // ( PP | MM ) Identifier [ '[' expression ']' ]
    GetToken( type, tokenStr, false, hasComment ) ; // get PP or MM
    GetToken( type, tokenStr, false, hasComment ) ; // want IDENT
    if ( type == IDENT ) {
      Variable identInfo ;
      IsIdDefined( tokenStr, identInfo ) ; // �o�̪�id����Ofunction
      PeekToken( type, tokenStr ) ; // peek whether is "["
      if ( type == LEFT_MID_PAREN )
        MidParenExpression() ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if
  else {
    GetToken( type, tokenStr, false, hasComment ) ;
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if

} // C_Interpreter::Unary_exp()

// signed_unary_exp
//     : Identifier [ '(' [ actual_parameter_list ] ')'
//                    |
//                    '[' expression ']'
//                  ]
//     | Constant
//     | '(' expression ')'
void C_Interpreter :: Signed_unary_exp() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ;

  if ( type == IDENT ) { // Identifier [ '(' [ actual_parameter_list ] ')' |  '[' expression ']' ]
    GetToken( type, tokenStr, false, hasComment ) ; // get IDENT
    string identName = tokenStr ;
    PeekToken( type, tokenStr ) ; // peek whether is "(" or  "["

    if ( type == LEFT_SAMLL_PAREN ) { // '(' [ actual_parameter_list ] ')'
      IsFunctionDefined( identName ) ;
      SmallParenOptionalActual_parameter_list() ;
    } // if
    else {
      Variable identInfo ;
      IsIdDefined( identName, identInfo ) ;
      if ( type == LEFT_MID_PAREN )  // '[' expression ']'
        MidParenExpression() ;
    } // end else

    return ;
  } // if
  else if ( type == CONSTANT ) {
    GetToken( type, tokenStr, false, hasComment ) ; // get CONSTANT
    return ;
  } // else if
  else if ( type == LEFT_SAMLL_PAREN ) { // '(' expression ')'
    SmallParenExpression() ;
  } // else if
  else {
    GetToken( type, tokenStr, false, hasComment ) ;
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if


} // C_Interpreter::Signed_unary_exp()


// unsigned_unary_exp
//     : Identifier [ '(' [ actual_parameter_list ] ')'
//                    |
//                    [ '[' expression ']' ] [ ( PP | MM ) ]
//                  ]
//     | Constant
//     | '(' expression ')'
void C_Interpreter :: Unsigned_unary_exp() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ;

  if ( type == IDENT ) {
    // Identifier [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ]
    GetToken( type, tokenStr, false, hasComment ) ; // get IDENT
    string identName = tokenStr ;
    Variable identInfo ;
    PeekToken( type, tokenStr ) ; // peek whether is "(" or  "[" or PP or MM
    if ( type == LEFT_SAMLL_PAREN ) { // '(' [ actual_parameter_list ] ')'
      IsFunctionDefined( identName ) ;
      SmallParenOptionalActual_parameter_list() ;
    } // if
    else {
      IsIdDefined( identName, identInfo ) ;
      if ( type == LEFT_MID_PAREN || type == PP || type == MM ) { // '[' expression ']'
        if ( type == LEFT_MID_PAREN ) {
          MidParenExpression() ;
          PeekToken( type, tokenStr ) ; // peek whether is PP or MM
        } // if

        if ( type == PP || type == MM )
          GetToken( type, tokenStr, false, hasComment ) ; // get PP or MM
      } // if
    } // end else

    return ;
  } // if
  else if ( type == CONSTANT ) {
    GetToken( type, tokenStr, false, hasComment ) ; // get CONSTANT
    return ;
  } // else if
  else if ( type == LEFT_SAMLL_PAREN ) { // '(' expression ')'
    SmallParenExpression() ;
  } // else if
  else {
    GetToken( type, tokenStr, false, hasComment ) ;
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // else if


} // C_Interpreter::Unsigned_unary_exp()

void C_Interpreter :: Semicolon() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ; // want ";"
  if ( type != SEMICOLON )
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
} // C_Interpreter::Semicolon()

//  : [ '[' Constant ']' ]
void C_Interpreter :: Optional_MidParen_Constant( bool & isArray, int & arraySize ) {

  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  PeekToken( type, tokenStr ) ; // peek whether is "["
  if ( type == LEFT_MID_PAREN ) {
    isArray = true ;
    GetToken( type, tokenStr, false, hasComment ) ; // get "["
    GetToken( type, tokenStr, false, hasComment ) ; // want CONSTANT

    bool isNum = true ;
    for ( int i = 0 ; i < tokenStr.size() ; i++ ) {
      if ( ! IsDigit( tokenStr[i] ) ) {
        isNum = false ;
        i = tokenStr.size() ;
      } // if
    } // for

    if ( type == CONSTANT && isNum ) {   // �}�C�A���̭����`�����ӥu��O���?

      arraySize = atoi( tokenStr.c_str() ) ;
      GetToken( type, tokenStr, false, hasComment ) ; // want "]"
      if ( type != RIGHT_MID_PAREN )
        throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
    } // if
    else
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else {
    isArray = false ;
    arraySize = 1 ;
  } // else

} // C_Interpreter::Optional_MidParen_Constant()

// '[' expression ']'
void C_Interpreter :: MidParenExpression() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ; // want "["
  if ( type == LEFT_MID_PAREN ) {
    Expression() ;
    GetToken( type, tokenStr, false, hasComment ) ; // want "]"
    if ( type != RIGHT_MID_PAREN )
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

} // C_Interpreter::MidParenExpression()

// '(' expression ')'
void C_Interpreter :: SmallParenExpression() {
  TokenType type ;
  string tokenStr ;
  bool hasComment ;

  GetToken( type, tokenStr, false, hasComment ) ; // want "("
  if ( type == LEFT_SAMLL_PAREN ) {
    Expression() ;
    GetToken( type, tokenStr, false, hasComment ) ; // want ")"
    if ( type != RIGHT_SAMLL_PAREN )
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;

} // C_Interpreter::SmallParenExpression()


// '(' [ actual_parameter_list ] ')'
void C_Interpreter :: SmallParenOptionalActual_parameter_list() {

  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ; // want "("
  if ( type == LEFT_SAMLL_PAREN ) {
    PeekToken( type, tokenStr ) ; // peek whether is actual_parameter_list start token
    if ( IsBasicExpressionBeginToken( type ) ) // actual_parameter_list �H basic_expression ���}�Y
      Actual_parameter_list() ;
    GetToken( type, tokenStr, false, hasComment ) ; // want ")"
    if ( type != RIGHT_SAMLL_PAREN )
      throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;
  } // if
  else
    throw new SyntacticError( tokenStr, mCommandLine, mNextChar ) ;


} // C_Interpreter::SmallParenOptionalActual_parameter_list()

void C_Interpreter :: InputBufferAppend() {


  TokenType type ;
  string tokenStr ;
  bool hasComment ;
  GetToken( type, tokenStr, false, hasComment ) ; // get "InputBufferAppend"
  GetNextChar( false ) ;

  tokenStr = "" ;

  while ( type != ENDINPUTBUFFERAPPEND ) {
    mInputBuffer = mInputBuffer + tokenStr  ;

    while ( ! IsAlpha( mNextChar ) ) { // ���O�^��r���}�Y�N������i�hbuffer��
      mInputBuffer += mNextChar ;
      GetNextChar( false ) ;
    } // while

    GetToken( type, tokenStr, false, hasComment ) ; // �O�^��r���}�Y�N�hget����token�A�~��P�_�O���O"EndInputBufferAppend"

  } // while

} // C_Interpreter::InputBufferAppend()


string C_Interpreter :: ReadInputFromBuffer( string type, string identName, bool isArray, int indexNum ) {

  string inputString = "" ;
  if ( type == "char" ) {
    if ( mInputBuffer.size() >= 1 ) {
      inputString += mInputBuffer[0] ;
      mInputBuffer.erase( mInputBuffer.begin() ) ;
      return inputString ;
    } // if
    else
      throw new NoInputError( identName, isArray, indexNum ) ;
  } // if
  else { // �nŪint float string bool���e�A����buffer�����Ů�Ū��
    int howManyWhiteSpace = 0 ;
    SkipWhiteSpaceInBuffer( howManyWhiteSpace ) ;
    if ( mInputBuffer.size() <= howManyWhiteSpace ) { // buffer�b�Ů椧��S���F��F-->error
      throw new NoInputError( identName, isArray, indexNum ) ;
    } // if

    int i = howManyWhiteSpace ;

    if ( type == "int" ) {
      if ( ! IsDigit( mInputBuffer[i] ) ) { // ���O�Ʀr
        throw new InputTypeError( identName, isArray, indexNum ) ;
      } // if
      else {
        while ( i < mInputBuffer.size() && IsDigit( mInputBuffer[i] ) ) {
          inputString += mInputBuffer[i] ;
          i++ ;
        } // while

        mInputBuffer.erase( mInputBuffer.begin(), mInputBuffer.begin() + i ) ;
        return inputString ;
      } // else
    } // if
    else if ( type == "float" ) {

      if ( ! IsDigit( mInputBuffer[i] ) && mInputBuffer[i] != '.' ) { // ���O�Ʀr�]���O�p���I
        throw new InputTypeError( identName, isArray, indexNum ) ;
      } // if
      else {
        bool hasDot = false ;
        while ( i < mInputBuffer.size() &&
                ( IsDigit( mInputBuffer[i] ) || ( mInputBuffer[i] == '.' && ! hasDot ) ) ) {
          if ( mInputBuffer[i] == '.' )
            hasDot = true ;
          inputString += mInputBuffer[i] ;
          i++ ;
        } // while

        mInputBuffer.erase( mInputBuffer.begin(), mInputBuffer.begin() + i ) ;
        return inputString ;
      } // else
    } // else if
    else if ( type == "string" ) { // Ū��ťլ���

      while ( i < mInputBuffer.size() && mInputBuffer[i] != ' ' && mInputBuffer[i] != '\t' &&
              mInputBuffer[i] != '\n' ) {
        inputString += mInputBuffer[i] ;
        i++ ;
      } // while

      mInputBuffer.erase( mInputBuffer.begin(), mInputBuffer.begin() + i ) ;
      return inputString ;
    } // else if
    else if ( type == "bool" ) { // �u��O"true"��"false"
      while ( i < mInputBuffer.size() && IsAlpha( mInputBuffer[i] ) ) {
        inputString += mInputBuffer[i] ;
        i++ ;
      } // while

      if ( inputString == "true" || inputString == "false" ) {
        mInputBuffer.erase( mInputBuffer.begin(), mInputBuffer.begin() + i ) ;
        return inputString ;
      } // if
      else
        throw new InputTypeError( identName, isArray, indexNum ) ;

    } // else if
  } // else

  return "" ;
} // C_Interpreter::ReadInputFromBuffer()

void C_Interpreter :: SkipWhiteSpaceInBuffer( int & howManyWhiteSpace ) {

  howManyWhiteSpace = 0 ;

  for ( int i = 0 ; i < mInputBuffer.size() ; i++ ) {
    if ( mInputBuffer[i] == ' ' || mInputBuffer[i] == '\t' || mInputBuffer[i] == '\n' ) {
      howManyWhiteSpace++ ;
    } // if
    else {
      return ;
    } // else
  } // for
} // C_Interpreter::SkipWhiteSpaceInBuffer()

void C_Interpreter :: InitializeVariable( Variable & var ) {

  // ��l�ƭȡA���P���O�n�����P?
  if ( var.isArray ) {
    if ( var.type == "string" )
      var.value.insert( var.value.begin(), var.arraySize, "\"\"" ) ; // �}�C������l�Ƭ�0�Avalue���s�Wvar.arraySize��0
    else if ( var.type == "char" )
      var.value.insert( var.value.begin(), var.arraySize, "''" ) ;
    else // "int" "float" "bool"
      var.value.insert( var.value.begin(), var.arraySize, "0" ) ;
  } // if
  else { // ���O�}�C
    if ( var.type == "string"  )
      var.value.insert( var.value.begin(), 1, "\"\"" ) ;
    else if ( var.type == "char" )
      var.value.insert( var.value.begin(), 1, "''" ) ;
    else
      var.value.insert( var.value.begin(), 1, "0" ) ;
  } // else

} // C_Interpreter::InitializeVariable()


// /************************************* Evaluation ********************************************///


void C_Interpreter :: Evaluate() {

  if ( mCurrentCommand.mIsStatement ) {
    vector<Token> stat = mCurrentCommand.mTokenList ;
    int i = 0 ;
    string caller ;
    Token result ;
    Evaluate_Statement( stat, i, caller, result ) ;
    // PrintVar();
  } // if

} // C_Interpreter::Evaluate()

void C_Interpreter :: Evaluate_Statement( vector<Token> stat, int & i, string caller, Token & result ) {
  /*
  cout << "Evaluate_Statement " << endl ;
  for ( int j = i ; j < stat.size() ; j++ )
    cout << stat[j].tokenStr << " " ;
  cout << endl ;
  */
  TokenType tt = stat[i].type ;

  if ( tt == SEMICOLON ) {
    return ;
  } // if
  else if ( IsBasicExpressionBeginToken( tt ) ) { // expression ';'  expression here should not be empty
    // expression �i��H IDENT, PP, MM, sign, "(" ���}�Y

    string needType = "" ;
    vector<Token> doExp ;
    stack<Token> operandStack ;
    stack<string> operatorStack ;
    for ( ; i < stat.size() && stat[i].type != SEMICOLON ; i++ ) { // ����";"����A�o��expression�A���h���B��
      doExp.push_back( stat[i] ) ;
    } // for

    Evaluate_Expression( doExp, operandStack, operatorStack, needType, caller ) ;
  } // else if
  else if ( tt == RETURN ) { // RETURN [ expression ] ';'
    i++ ; // "return"�᪺�Ĥ@��token
    vector<Token> returnExp ;
    for ( ; i < stat.size() && stat[i].type != SEMICOLON ; i++ )
      returnExp.push_back( stat[i] ) ;
    stack<Token> returnOperand ;
    stack<string> returnOperator ;
    string needType = "" ;
    if ( ! returnExp.empty() ) {
      Evaluate_Expression( returnExp, returnOperand, returnOperator, needType, caller ) ;
      string type, value ;
      GetTokenValue( returnOperand.top(), type, value, caller ) ;
      result.tokenStr = value ;
      result.type = CONSTANT ;
    } // if

    i = stat.size() ;
  } // else if
  else if ( tt == LEFT_BIG_PAREN ) { // compound_statement--> '{' { declaration | statement } '}'
    if ( caller.empty() ) // ���O�bfunction��
      mLevel++ ; // �J�쥪�A�� ���h+1
    else {
      Function funcInfo ;
      GetFunctionInfo( caller, funcInfo ) ;
      funcInfo.level++ ;
      UpdateFunction( caller, funcInfo ) ;
    } // end else

    i++ ; // "{"�᪺��1��token

    for (  ; i < stat.size() && stat[i].type != RIGHT_BIG_PAREN ; i++ ) {
      TokenType type = stat[i].type ;
      if ( Type_specifier( type ) ) { // declaration
        InCompoundDeclaration( stat, i, caller ) ;
      } // if
      else if ( type == SEMICOLON || IsBasicExpressionBeginToken( type ) || type == RETURN ||
                type == LEFT_BIG_PAREN || type == IF || type == WHILE || type == DO  ) { // statement���}�Y
        Evaluate_Statement( stat, i, caller, result ) ;
      } // else if
    } // for

    if ( caller.empty() ) { // ���O�bfunction��
      if ( mLevel == mVarTable.size() -1 ) // �b�o�@�h���ŧi�ܼơA�h�b�k�j�A������N����ΤF
        mVarTable.pop_back() ;
      mLevel-- ; // �J��k�A�� ���h-1
    } // if
    else {
      Function funcInfo ;
      GetFunctionInfo( caller, funcInfo ) ;
      if ( funcInfo.level == funcInfo.funcDefineVar.size() -1 ) // �b�o�@�h���ŧi�ܼơA�h�b�k�j�A������N����ΤF
        funcInfo.funcDefineVar.pop_back() ;
      funcInfo.level--;
      UpdateFunction( caller, funcInfo ) ;
    } // end else
  } // else if
  else if ( tt == IF ) { // IF '(' expression ')' statement [ ELSE statement ]
    i++ ; // "("
    vector<Token> parenExp ;
    GetInParenExp( stat, i, parenExp ) ; // ���o�A������exp
    stack<Token> parenOperand ;
    stack<string> parenOperator ;
    string needType ;
    i++ ; // ")" �᪺�Ĥ@��token
    vector<Token> ifStatment, elseStatment ;
    bool hasElse = false ;
    GetIfWhileStatement( stat, i, ifStatment ) ; // ���oif����true�ɭn�����Ҧ�statement
    if ( i+1 < stat.size() && stat[i+1].type == ELSE ) {
      hasElse = true ;
      i += 2 ; // i+1->"else" i+2->"else"�᪺��1��token
      GetIfWhileStatement( stat, i, elseStatment ) ; // ���oif����true�ɭn�����Ҧ�statement
    } // if

    Evaluate_Expression( parenExp, parenOperand, parenOperator, needType, caller ) ; // �B��n�P�_������
    string type, value ;
    GetTokenValue( parenOperand.top(), type, value, caller ) ;
    if ( type == "char" || type == "string" )
      value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸��γ�޸�
    float ifResult = atof( value.c_str() ) ;
    if ( ifResult ) {
      int j = 0 ;
      Evaluate_Statement( ifStatment, j, caller, result ) ;
    } // if
    else if ( hasElse ) {
      int j = 0 ;
      Evaluate_Statement( elseStatment, j, caller, result ) ;
    } // else if

  } // else if
  else if ( tt == WHILE ) { // WHILE '(' expression ')' statement
    i++ ; // "("
    vector<Token> parenExp, whileStatment ;
    GetInParenExp( stat, i, parenExp ) ; // ���o�A������exp
    stack<Token> parenOperand ;
    stack<string> parenOperator ;
    string needType ;
    i++ ; // ")" �᪺�Ĥ@��token
    GetIfWhileStatement( stat, i, whileStatment ) ; // ���owhile����true�ɭn�����Ҧ�statement

    bool repeat = true ;
    do {
      ClearStack( parenOperand, parenOperator ) ;
      Evaluate_Expression( parenExp, parenOperand, parenOperator, needType, caller ) ; // �B��n�P�_������
      string type, value ;
      GetTokenValue( parenOperand.top(), type, value, caller ) ;
      if ( type == "char" || type == "string" )
        value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸��γ�޸�
      float whileResult = atof( value.c_str() ) ;
      if ( whileResult ) {
        int j = 0 ;
        Evaluate_Statement( whileStatment, j, caller, result ) ;
      } // if
      else {
        repeat = false ;
      } // end else

    } while ( repeat ) ;

  } // else if
  else if ( tt == DO ) { // DO statement WHILE '(' expression ')' ';'
    i++ ; // do �᪺�Ĥ@��token
    vector<Token> parenExp, doStatment ;
    GetIfWhileStatement( stat, i, doStatment ) ;
    i += 2 ; // i+1->"while"  i+2->"("
    GetInParenExp( stat, i, parenExp ) ; // ���o�A������exp
    i++ ; // ";"
    bool repeat = true ;
    do {
      int j = 0 ;
      Evaluate_Statement( doStatment, j, caller, result ) ;
      stack<Token> parenOperand ;
      stack<string> parenOperator ;
      string needType, type, value ;
      Evaluate_Expression( parenExp, parenOperand, parenOperator, needType, caller ) ;
      GetTokenValue( parenOperand.top(), type, value, caller ) ;
      if ( type == "char" || type == "string" )
        value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸��γ�޸�
      float whileResult = atof( value.c_str() ) ;
      if ( whileResult )
        repeat = true ;
      else
        repeat = false ;
    } while ( repeat ) ;

  } // else if


} // C_Interpreter::Evaluate_Statement()

void C_Interpreter :: GetIfWhileStatement( vector<Token> stat, int & i, vector<Token> & doStat ) {

  if ( stat[i].type == LEFT_BIG_PAREN ) { // "{"--> ���������"}"����
    doStat.push_back( stat[i] ) ; // "{"
    int parenNum = 1 ;
    i++ ; // "{" �᪺�Ĥ@��token
    for ( ; parenNum != 0 ; i++ ) {
      if ( stat[i].type == LEFT_BIG_PAREN )
        parenNum++ ;
      else if ( stat[i].type == RIGHT_BIG_PAREN )
        parenNum-- ;
      doStat.push_back( stat[i] ) ;
    } // for

    i-- ; // "}"
  } // if
  else if ( stat[i].type == IF ) { // IF '(' expression ')' statement [ ELSE statement ]
    doStat.push_back( stat[i] ) ; // if
    doStat.push_back( stat[++i] ) ; // "("
    GetInParenExp( stat, i, doStat ) ; // ���o�A������exp
    doStat.push_back( stat[i] ) ; // ")"
    i++ ; // ")" �᪺��1��token
    GetIfWhileStatement( stat, i, doStat ) ;

    if ( stat[i+1].type == ELSE ) {
      i++ ;
      doStat.push_back( stat[i] ) ; // else
      i++ ;
      GetIfWhileStatement( stat, i, doStat ) ;
    } // if

  } // else if
  else if ( stat[i].type == WHILE ) { // WHILE '(' expression ')' statement
    doStat.push_back( stat[i] ) ; // while
    doStat.push_back( stat[++i] ) ; // "("
    GetInParenExp( stat, i, doStat ) ; // ���o�A������exp
    doStat.push_back( stat[i] ) ; // ")"
    i++ ;  // ")" �᪺��1��token
    GetIfWhileStatement( stat, i, doStat ) ;
  } // else if
  else if ( stat[i].type == DO ) { // DO statement WHILE '(' expression ')' ';'
    doStat.push_back( stat[i] ) ; // do
    i++ ;
    GetIfWhileStatement( stat, i, doStat ) ;
    i++ ;
    doStat.push_back( stat[i] ) ; // while
    doStat.push_back( stat[++i] ) ; // "("
    GetInParenExp( stat, i, doStat ) ; // ���o�A������exp
    doStat.push_back( stat[i] ) ; // ")"
    doStat.push_back( stat[++i] ) ; // ";"
  } // else if
  else { // �����������
    for ( ; stat[i].type != SEMICOLON ; i++ ) {
      doStat.push_back( stat[i] ) ;
    } // for

    doStat.push_back( stat[i] ) ; // ";"
  } // else
} // C_Interpreter::GetIfWhileStatement()

void C_Interpreter :: InCompoundDeclaration( vector<Token> stat, int & i, string caller ) {
  // �ϰ��ܼ�

  vector<Variable> localVar ;
  string identType = stat[i].tokenStr ; // type_specifier�A�ŧi�����O
  i++ ; // ident�A�ŧi���ܼƦW��
  for ( ; stat[i].type != SEMICOLON ; i++ ) {
    Variable newVar ; // �s�ܼ�
    newVar.name = stat[i].tokenStr ;
    newVar.type = identType ;
    newVar.level = mLevel ;
    if ( stat[i+1].type == LEFT_MID_PAREN ) { // '[' Constant ']'
      i++ ; // "["
      newVar.isArray = true ;
      newVar.arraySize = atoi( stat[++i].tokenStr.c_str() ) ;
      i++ ; // "]"
    } // if
    else {
      newVar.isArray = false ;
      newVar.arraySize = 1 ;
    } // else

    InitializeVariable( newVar ) ; // ��l���ܼƭ�
    localVar.push_back( newVar ) ;

    if ( stat[i+1].type == COMMA )
      i++ ;
  } // for

  if ( caller.empty() )
    StoreLocalVariable( localVar ) ;
  else {
    Function func ;
    GetFunctionInfo( caller, func ) ;
    StoreFunctionVar( func, localVar ) ;
    UpdateFunction( caller, func ) ;
  } // end else
} // C_Interpreter::InCompoundDeclaration()

void C_Interpreter :: StoreFunctionVar( Function & func, vector<Variable> funcVar ) {

  if ( func.level >= func.funcDefineVar.size() ) { // mVarTable�S���{�b�h�ƪ��ܼƪ�
    while ( func.level >= func.funcDefineVar.size() ) { //
      map<string, Variable> newLevel ;
      func.funcDefineVar.push_back( newLevel ) ;
    } // while
  } // if

  for ( int i = 0 ; i < funcVar.size() ; i++ ) {
    map<string, Variable>::iterator iter ;
    iter = func.funcDefineVar[func.level].find( funcVar[i].name ) ;
    if ( iter != func.funcDefineVar[func.level].end() ) { // �o�@�h���@�˦W�٪��ܼ�
      iter->second = funcVar[i] ;
    } // if
    else {
      pair<string, Variable> aVariable ;
      aVariable.first = funcVar[i].name ;
      aVariable.second = funcVar[i] ;
      func.funcDefineVar[func.level].insert( aVariable ) ;
    } // else
  } // for

} // C_Interpreter::StoreFunctionVar()



// basic_expression { ',' basic_expression }
void C_Interpreter :: Evaluate_Expression( vector<Token> stat, stack<Token> & operandStack,
                                           stack<string> & operatorStack, string needType,
                                           string caller ) {
  /*
  cout << "Evaluate_Expression : " ;
  for ( int i = 0 ; i < stat.size() ; i++ ) {
    cout << stat[i].tokenStr << " " ;
  } // for
  cout << endl ;
  */
  int i = 0 ;
  TokenType tt = stat[i].type ;

  while ( i < stat.size() ) { //

    vector<Token> calToken ;
    needType = "" ;
    ClearStack( operandStack, operatorStack ) ; // �M��stack
    int parenNum = 0 ;

    // ���o�@��basic_expression
    while ( i < stat.size() && ( tt != COMMA || parenNum != 0 ) ) {
      if ( stat[i].type == LEFT_SAMLL_PAREN )
        parenNum++ ;
      else if ( stat[i].type == RIGHT_SAMLL_PAREN )
        parenNum-- ;
      calToken.push_back( stat[i] ) ;
      i++ ;
      if ( i < stat.size() )
        tt = stat[i].type ;
    } // while

    Evaluate_Basic_Expression( calToken, operandStack, operatorStack, needType, caller ) ;


    if ( tt == COMMA ) {
      tt = stat[++i].type ; // "," �᪺�Ĥ@��token
    } // if
  } // while

} // C_Interpreter::Evaluate_Expression()

void C_Interpreter :: ClearStack( stack<Token> & operandStack, stack<string> & operatorStack ) {
  while ( ! operandStack.empty() )
    operandStack.pop() ;
  while ( ! operatorStack.empty() )
    operatorStack.pop() ;
} // C_Interpreter::ClearStack()

void C_Interpreter :: Evaluate_Basic_Expression( vector<Token> calToken, stack<Token> & operandStack,
                                                 stack<string> & operatorStack, string needType,
                                                 string caller ) {
  /*
  cout << "Evaluate_Basic_Expression" << endl ;
  for ( int i = 0 ; i < calToken.size() ; i++ )
    cout << calToken[i].tokenStr << " " ;
  cout << endl ;
  */
  for ( int i = 0 ; i < calToken.size() ; i++ ) {

    TokenType tt = calToken[i].type ;

    if ( tt == IDENT ) {
      Variable identInfo ;
      string identName = calToken[i].tokenStr ;
      Token preToken ;
      int indexNum ;
      Identifier( identName, calToken, i, preToken, needType, operandStack, operatorStack, identInfo,
                  indexNum, caller ) ;
      // cout << "Ident : " << identName << " End ! " << endl ;
    } // if
    else if ( tt == CONSTANT ) { // �`�� : "aaa"�B'a'�B58�B45�Btrue�Bfalse
      operandStack.push( calToken[i] ) ;
    } // else if
    else if ( Assignment_operator( tt ) ) { // assign���u���v�̧C�A�B��right�A�ҥH������i�B��l��stack
      operatorStack.push( calToken[i].tokenStr );
    } // else if
    else if ( tt == PP || tt == MM ) { // ��++ --�Aident���[�δ�A�i��B��
      Token ppmm = calToken[i] ;
      i++ ; // ���o���bppmm�᭱��ident token
      if ( calToken[i].type == IDENT ) {
        Variable identInfo ;
        int indexNum ;
        Identifier( calToken[i].tokenStr, calToken, i, ppmm, needType, operandStack, operatorStack,
                    identInfo, indexNum, caller ) ;
        PPMM( ppmm, identInfo, indexNum, caller ) ;
        Token ppmmNum ;
        ppmmNum.tokenStr = identInfo.value[indexNum] ;
        ppmmNum.type = CONSTANT ;
        operandStack.push( ppmmNum ) ;
      } // if
    } // else if
    else if ( tt == PLUS || tt == MINUS || tt == MULTIPLICAION || tt == DIVISION || tt == PERCENT ||
              tt == EQUAL || tt == NOTEQUAL || tt == LESS || tt == GREATER || tt == GE || tt == LE ||
              tt == LS || tt == RS || tt == AND || tt == OR ) { // �B��l�A��stack�����B��l���u���v�����
      Operator( calToken[i].tokenStr, operandStack, operatorStack, needType, caller ) ;
    } // else if
    else if ( tt == QUES ) { // ? :
      QuestionMark( calToken, i, operandStack, operatorStack, needType, caller ) ;
    } // else if
    else if ( tt == SIGN ) {  // + - !
      Sign( calToken, i, operandStack, operatorStack, needType, caller ) ;
    } // else if
    else if ( tt == LEFT_SAMLL_PAREN ) { // �J�쥪�A���A��b�A�������B�⦡���h�B��
      // operatorStack.push( "(" ) ;
      vector<Token> parenExp ;
      stack<Token> parenOperand ;
      stack<string> parenOperator ;
      GetInParenExp( calToken, i, parenExp ) ;
      Evaluate_Expression( parenExp, parenOperand, parenOperator, needType, caller ) ;
      operandStack.push( parenOperand.top() ) ;
    } // else if
    else if ( tt == CIN ) {
      Cin( calToken, i, caller ) ;
    } // else if
    else if ( tt == COUT ) {
      Cout( calToken, i, operandStack, operatorStack, needType, caller ) ;
    } // else if

  } // for

  CalRestInStack( operandStack, operatorStack, needType, caller ) ; // �N�ѤU�bstack�̪�����

} // C_Interpreter::Evaluate_Basic_Expression()

void C_Interpreter :: GetInParenExp( vector<Token> calToken, int & i, vector<Token> & parenExp ) {

  int parenNum = 1 ; // ���A���ƶq
  i++ ; // ���A���᪺�Ĥ@��token
  for ( ; i < calToken.size() && parenNum != 0 ; i++ ) {
    if ( calToken[i].type == LEFT_SAMLL_PAREN )
      parenNum++ ;
    else if ( calToken[i].type == RIGHT_SAMLL_PAREN )
      parenNum-- ;
    parenExp.push_back( calToken[i] ) ;
  } // for

  parenExp.pop_back() ; // ��̫᪺")"�R��
  i-- ; // �]���W���j��|��i�h�[��1

} // C_Interpreter::GetInParenExp()


void C_Interpreter :: Cin( vector<Token> calToken, int & i, string caller ) {

  TokenType tt = calToken[++i].type ; // get ">>"

  while ( tt == RS ) { // ">>"

    string varName = calToken[++i].tokenStr ;
    int indexNum = 0 ;
    Variable varInfo ;
    bool funcPara = false, byFuncRef = false, byRef = false ;
    string referFunc, referVar ;
    GetVariableInfo( varName, varInfo, caller, indexNum, funcPara, byRef, byFuncRef, referFunc, referVar ) ;
    bool isChar = false ;

    if ( i+1 < calToken.size() && calToken[i+1].type == LEFT_MID_PAREN ) { // array

      i += 2 ; // ���A���᪺�Ĥ@��token
      string indexExp ;
      CalArrayIndexExpression( calToken, i, indexExp, indexNum, caller ) ;

      if ( indexNum < 0 || indexNum >= varInfo.arraySize ) // index�ȶW�L�d��
        throw new ArrayIndexOutofRangeError( varName, indexExp, indexNum ) ;
    } // if
    else {
      if ( varInfo.isArray && varInfo.type == "char" ) {  // char array�i�H
        isChar = true ;
        for ( int j = 0 ; j < varInfo.arraySize ; j++ ) {
          if ( j == 0 )
            varInfo.value[j] = ReadInputFromBuffer( "char", varName, true, j ) ;
          else if ( mInputBuffer.size() > 0 )
            varInfo.value[j] = ReadInputFromBuffer( "char", varName, true, j ) ;
          else
            j = varInfo.arraySize ;
        } // for
      } // if

    } // else

    if ( ! isChar )
      varInfo.value[indexNum] = ReadInputFromBuffer( varInfo.type, varName, varInfo.isArray, indexNum ) ;
    UpdateVariable( varName, varInfo, caller ) ;

    if ( i + 1 < calToken.size() ) {
      tt = calToken[++i].type ; // get ">>"
    } // if
    else
      return ;
  } // while

} // C_Interpreter::Cin()

void C_Interpreter :: Cout( vector<Token> calToken, int & i, stack<Token> & operandStack,
                            stack<string> & operatorStack, string & needType,
                            string caller ) {

  TokenType tt = calToken[++i].type ; // get "<<"

  while ( tt == LS ) {
    i++ ; // "<<" �᪺�Ĥ@��token
    vector<Token> coutExp ;

    int parenNum = 0 ;
    // eg. ( cout << 5 ) , cout << (5)
    for (  ; i < calToken.size() && ( calToken[i].type != LS || parenNum != 0 ) ; i++ ) {
      if ( calToken[i].type == LEFT_SAMLL_PAREN ) {
        parenNum++ ; // �b "<<" �ᦳ "("
      } // if
      else if ( calToken[i].type == RIGHT_SAMLL_PAREN ) {
        parenNum-- ;
      } // else if

      coutExp.push_back( calToken[i] ) ;
    } // for

    stack<Token> coutOperand ;
    stack<string> coutOperator ;
    string coutNeedType, coutStr ;

    Evaluate_Expression( coutExp, coutOperand, coutOperator, coutNeedType, caller ) ;

    coutStr = coutOperand.top().tokenStr ;
    PrintCout( coutOperand.top(), operandStack, caller ) ;

    if ( i < calToken.size() )
      tt = calToken[i].type ; // get "<<"
    else
      return ;

  } // while

} // C_Interpreter::Cout()

void C_Interpreter :: PrintCout( Token coutToken, stack<Token> & operandStack,
                                  string caller ) {


  string type, value ;
  GetTokenValue( coutToken, type, value, caller ) ;

  Token returnToken ;
  returnToken.type = CONSTANT ;

  if ( type == "string" || type == "char" ) {
    returnToken.tokenStr =  value ;
    // if ( type == "string" )
    //   returnToken.tokenStr = "\"" + value + "\"" ;
    // else
    //   returnToken.tokenStr = "'" + value + "'" ;

    value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸��γ�޸�
    for ( int i = 0 ; i < value.size() ; i++ ) {
      if ( value[i] == '\\' ) { // ����r��
        if ( i + 1 < value.size() ) {

          if ( value[i+1] == 'n' )
            cout << "\n" ;
          else if ( value[i+1] == 't' )
            cout << "\t" ;
          else {
            string escapeStr = value.substr( i, 2 );
            cout << escapeStr ;
          } // else

          i++ ;

        } // if
        else {
          // throw new MissTerminateCh( coutToken.tokenStr[0] ) ;
        } // else
      } // if
      else
        cout <<  value[i] ;
    } // for

  } // if
  else if ( type == "bool" ) {
    if ( value == "0" ) {
      returnToken.tokenStr = "false" ;
      cout << "false" ;
    } // if
    else {
      returnToken.tokenStr = "true" ;
      cout << "true" ;
    } // end else
  } // else if
  else { // "int" or "float"

    int dot = value.find( "." ) ;
    if ( dot != -1 ) { // �����p���I

      bool isFloat = false ;
      for ( int i = dot+1 ; i < value.size() ; i++ ) {
        if ( value[i] != '0' ) {
          isFloat = true ;
          i = value.size() ; // ���Xfor
        } // if
      } // for

      if ( ! isFloat )
        value = value.substr( 0, dot ) ;
    } // if

    cout << value ;
    returnToken.tokenStr = value ;
  } // end else

  operandStack.push( returnToken ) ;
} // C_Interpreter::PrintCout()

void C_Interpreter :: Identifier( string identName, vector<Token> calToken, int & i, Token preToken,
                                  string & needType, stack<Token> & operandStack,
                                  stack<string> & operatorStack, Variable & identInfo, int & indexNum,
                                  string caller ) {

  // GetVariableInfo( identName, identInfo ) ; // ���o�o���ܼƪ���T
  indexNum = 0 ;
  int referenceIndex = 0 ;

  // cout << "ident Name : " << identName << endl ;

  if ( i+1 < calToken.size() &&  calToken[i+1].type == LEFT_MID_PAREN ) {  // array
    bool funcPara = false, byFuncRef = false, byRef = false ;
    string referFunc, referVar ;
    GetVariableInfo( identName, identInfo, caller, referenceIndex,
                     funcPara, byRef, byFuncRef, referFunc, referVar ) ; // ���o�o���ܼƪ���T

    // if ( ! identInfo.isArray ) // �o���ܼƤ��O�Q�ŧi��array-->���|���o�ؿ�
    //  throw new InvalidTypeError() ;

    i += 2 ; // i+1->"["  i+2->���A���᪺�Ĥ@��token

    string indexExp ; // �x�s���A���̭����B�⦡�A�Lerror�ɥ�

    CalArrayIndexExpression( calToken, i, indexExp, indexNum, caller ) ; // �p��array��index�O�h��

    if ( i + 1 < calToken.size() && Assignment_operator( calToken[i+1].type ) )
      needType = identInfo.type ; // �U�@�ӬOassign�A�N��ݭn�����O���o��ident�����O

    // ��array���S���o��index
    // cout << "ident : " << identInfo.name << "  [" << identInfo.arraySize << "]" << endl;
    if ( indexNum >= 0 &&  indexNum < identInfo.arraySize ) { // �d�򥿽T

      string arrayValue = identInfo.value[indexNum] ;

      if ( preToken.type == PP || preToken.type == MM || preToken.type == SIGN ) { // �e�@��token�O++ -- �� + - !
        return ; // �^�h��
      } // if
      else { // �e���S��++ -- �� + - !��array
        // �ݫ᭱���S��++ --
        if ( i+1 < calToken.size() &&  ( calToken[i+1].type == PP || calToken[i+1].type == MM ) ) {
          // �᭱��++--
          Token arrayPpmm ; // pp mm �b�ܼƫ᭱�A���B��έ쥻���Ȱ�
          arrayPpmm.tokenStr = arrayValue ;
          arrayPpmm.type = CONSTANT ;
          operandStack.push( arrayPpmm ) ;
          i++ ; // PP MM
          PPMM( calToken[i], identInfo, indexNum, caller ) ;
        } // if
        else { // �᭱�S��++ --
          Token arrayToken ;
          arrayToken.tokenStr = identName + "[" + NumToString( indexNum, false ) + "]" ;
          arrayToken.type = IDENT_ARRAY ;
          operandStack.push( arrayToken ) ;
        } // else
      } // else
    } // if
    else { // array index�d�򤣥��T
      throw new ArrayIndexOutofRangeError( identName, indexExp, indexNum ) ;
    } // else

  } // if
  else if ( i+1 < calToken.size() &&  calToken[i+1].type == LEFT_SAMLL_PAREN ) { // function project4

    i += 2 ; // i+1-> "("  i+2-> "("�᪺��1��token

    CallFunction( identName, calToken, i, caller, operandStack ) ;

  } // else if
  else { // ���O�}�C �]���Ofunction
    bool funcPara = false, byFuncRef = false, byRef = false ;
    string referFunc, referVar ;
    GetVariableInfo( identName, identInfo, caller, referenceIndex,
                     funcPara, byRef, byFuncRef, referFunc, referVar ) ; // ���o�o���ܼƪ���T

    if ( i + 1 < calToken.size() && Assignment_operator( calToken[i+1].type ) )
      needType = identInfo.type ; // �U�@�ӬOassign�A�N��ݭn�����O���o��ident�����O

    if ( preToken.type == PP || preToken.type == MM || preToken.type == SIGN ) {
      return ;
    } // if
    else { // �e���S��++ -- �� + - !�� ident

      if ( i+1 < calToken.size() &&  ( calToken[i+1].type == PP || calToken[i+1].type == MM ) ) {
        // ident�᭱��++--
        Token original ;
        original.tokenStr = identInfo.value[referenceIndex] ; // 0��referenceIndex
        original.type = CONSTANT ;
        operandStack.push( original ) ;
        i++ ; // PP MM
        PPMM( calToken[i], identInfo, 0, caller ) ;
      } // if
      else {
        operandStack.push( calToken[i] ) ;
        // cout << "push : " << calToken[i].tokenStr << endl;
      } // else
    } // else
  } // else
} // C_Interpreter::Identifier()

void C_Interpreter :: CallFunction( string funcName, vector<Token> calToken, int & i,
                                    string caller, stack<Token> & operandStack ) {

  // '(' [ actual_parameter_list ] ')'   ��i����")"
  Function currentFunc ; // �{�b�n�I�s��function����T
  GetFunctionInfo( funcName, currentFunc ) ;

  int outerParenNum = 1 ; // �̥~�򪺥��A��
  TokenType tt = calToken[i].type ; // "(" �᪺�Ĥ@��token
  int paraNum = 0 ; // �ĴX�ӰѼ�

  while ( i < calToken.size() && outerParenNum != 0 ) { //

    vector<Token> paraExp ; // �x�s�Ѽƪ��B�⦡
    stack<Token> paraOperand ;
    stack<string> paraOperator ;
    string needType = "" ;
    int innerParenNum = 0 ;

    // ���o�@��basic_expression
    while ( i < calToken.size() && outerParenNum != 0 && ( tt != COMMA || innerParenNum != 0 ) ) {
      if ( tt == LEFT_SAMLL_PAREN ) {
        innerParenNum++ ;
        paraExp.push_back( calToken[i] ) ;
      } // if
      else if ( tt == RIGHT_SAMLL_PAREN && innerParenNum > 0 ) {
        innerParenNum++ ;
        paraExp.push_back( calToken[i] ) ;
      } // else if
      else if ( tt == RIGHT_SAMLL_PAREN && innerParenNum <= 0 ) {
        outerParenNum-- ;
      } // else if
      else
        paraExp.push_back( calToken[i] ) ;
      i++ ;
      if ( i < calToken.size() )
        tt = calToken[i].type ;
    } // while

    if ( ! paraExp.empty() ) {
      Evaluate_Basic_Expression( paraExp, paraOperand, paraOperator, needType, caller ) ;

      // formal parameter��T
      Variable paraInfo ; // ���o�o�ӰѼƪ���T�A�ݬO���Ocall by reference�άOarray
      int index = 0 ;
      bool funcPara = false, byFuncRef = false, byRef = false ;
      string referFunc, referVar ;
      GetVariableInfo( currentFunc.parameterList[paraNum].name, paraInfo, funcName, index,
                       funcPara, byRef, byFuncRef, referFunc, referVar ) ;

      if ( paraInfo.isArray || paraInfo.passByReference ) { // call by reference
        int index = 0 ;
        string referenceVar = paraOperand.top().tokenStr ;
        if ( paraOperand.top().type == IDENT_ARRAY ) {
          int leftParen = paraOperand.top().tokenStr.find( "[" ) ;
          int rightParen = paraOperand.top().tokenStr.size() - 1 ;
          referenceVar = paraOperand.top().tokenStr.substr( 0, leftParen ) ;
          index = atoi( paraOperand.top().tokenStr.substr( leftParen+1, rightParen-leftParen-1 ).c_str() ) ;
        } // if
        // cout << "reference to : " << referenceVar << endl ;

        Variable actualPara ;
        int referenceIndex = 0 ;
        funcPara = false, byFuncRef = false, byRef = false ;
        referFunc = "", referVar = " " ;
        GetVariableInfo( referenceVar, actualPara, caller, referenceIndex,
                         funcPara, byRef, byFuncRef, referFunc, referVar ) ; // actual para�ѦҨ쪺�ܼƪ���T

        if ( byFuncRef ) { // �O�ѦҨ�caller���ѼơA
          paraInfo.referenceFunction = referFunc ;
          paraInfo.referenceVar = referVar ;
        } // if
        else if ( byRef ) { // �ѦҨ�function���ѼơA�ӳo�ӰѼưѦҨ�����ܼƩΰϰ��ܼ�
          paraInfo.referenceLevel = actualPara.level ;
          paraInfo.referenceVar = actualPara.name ;
          paraInfo.referenceIndex = referenceIndex ;
        } // else if
        else if ( funcPara ) { // �ѦҨ�function���ѼơA�ӳo�ӰѼƨS���ѦҨ��L�ܼ�
          paraInfo.referenceVar = referenceVar ;
          paraInfo.referenceFunction = caller ;
        } // else if
        else { // �ѦҨ�����ܼƩΰϰ��ܼ�
          paraInfo.referenceLevel = actualPara.level ;
          paraInfo.referenceVar = referenceVar ;
          paraInfo.referenceIndex = index ;
        } // end else
      } // if
      else { // call by value
        string type, value ;
        GetTokenValue( paraOperand.top(), type, value, caller ) ;
        paraInfo.value.push_back( value ) ;
      } // else

      UpdateVariable( currentFunc.parameterList[paraNum].name, paraInfo, funcName ) ;
      paraNum++ ;
      if ( tt == COMMA ) {
        tt = calToken[++i].type ; // "," �᪺�Ĥ@��token
      } // if
    } // if


  } // while

  i-- ;
  int j = 0 ;

  Token result ;
  Evaluate_Statement( currentFunc.body, j, funcName, result ) ;
  if ( currentFunc.returnType != "void" ) {
    operandStack.push( result ) ;
  } // if

  FunctionDone( funcName ) ;


} // C_Interpreter::CallFunction()


// ��X�}�C������
void C_Interpreter :: CalArrayIndexExpression( vector<Token> calToken, int & i, string & indexExp,
                                                int & indexNum, string caller ) {

  vector<Token> index ;  // �p��Xarray�����ު��B�⦡token
  string indexResult ;
  indexExp = "" ; // array�����޹B�⦡ eg: [ a+5 ]�A �sa+5�A�Lerror�ɷ|�Ψ�

  int parenNum = 1 ;
  for ( ; parenNum != 0 ; i++ ) {  // ���i��a[ b[4] ]
    if ( calToken[i].type == LEFT_MID_PAREN )
      parenNum++ ;
    else if ( calToken[i].type == RIGHT_MID_PAREN )
      parenNum-- ;
    indexExp += calToken[i].tokenStr ;
    index.push_back( calToken[i] ) ;
  } // for

  i-- ; // �W���j��|��i����"]"�᪺��1��token
  indexExp = indexExp.substr( 0, indexExp.size()-1 ) ;  // ��̫᪺"]"����
  index.pop_back() ; // ��̫᪺"]"����
  stack<Token> indexOperand ;
  stack<string> indexOperator ;
  Evaluate_Basic_Expression( index, indexOperand, indexOperator, "int", caller ) ;
  indexResult = indexOperand.top().tokenStr ;
  string type ;
  GetTokenValue( indexOperand.top(), type, indexResult, caller ) ; // indexResult���ӭn�O���
  if ( type == "char" || type == "string" )
    indexResult = indexResult.substr( 1, indexResult.size()-2 ) ; // �h���Y�������޸��γ�޸�
  indexNum = atoi( indexResult.c_str() ) ;

} // C_Interpreter::CalArrayIndexExpression()


void C_Interpreter :: PPMM( Token ppmm, Variable & identInfo, int indexNum, string caller ) {

  string originalValue = identInfo.value[indexNum] ;
  // if ( identInfo.type != "int" && identInfo.type != "float" ) // ���Oint��float ���వ++ �� --
  //  throw EvaluationError( "int", identInfo.name ) ;

  if ( ppmm.type == PP ) {
    if ( identInfo.type == "int" )
      identInfo.value[indexNum] = NumToString( atoi( originalValue.c_str() ) + 1, false ) ;
    else
      identInfo.value[indexNum] = NumToString( atof( originalValue.c_str() ) + 1, true ) ;
  } // if
  else {
    if ( identInfo.type == "int" )
      identInfo.value[indexNum] = NumToString( atoi( originalValue.c_str() ) - 1, false ) ;
    else
      identInfo.value[indexNum] = NumToString( atof( originalValue.c_str() ) - 1, true ) ;
  } // else

  UpdateVariable( identInfo.name, identInfo, caller ) ; // ��s�o���ܼƪ���T

} // C_Interpreter::PPMM()

void C_Interpreter :: Sign( vector<Token> calToken, int & i, stack<Token> & operandStack,
                            stack<string> & operatorStack, string & needType, string caller ) {

  Token signToken = calToken[i] ;
  string signStr = calToken[i].tokenStr ; // "+" "-" "!"
  Token signNum ;

  i++ ; // ��sign���᪺token�O����


  bool isConstant = false ;
  string nextStr = calToken[i].tokenStr ;
  string type ;
  if ( calToken[i].type == LEFT_SAMLL_PAREN ) { // �q���A����k�A����token���h�B��
    isConstant = true ;
    vector<Token> parenExp ;
    GetInParenExp( calToken, i, parenExp ) ;
    stack<Token> parenOperand ;
    stack<string> parenOperator ;
    Evaluate_Basic_Expression( parenExp, parenOperand, parenOperator, needType, caller ) ;
    GetTokenValue( parenOperand.top(), type, nextStr, caller ) ;
  } // if
  else if ( calToken[i].type == IDENT ) { // sign�᭱�O���ܼ�
    Variable identInfo ;
    int indexNum = 0 ;
    string signValue ;
    Identifier( calToken[i].tokenStr, calToken, i, signToken,  needType, operandStack, operatorStack,
                identInfo, indexNum, caller ) ;
    SignIdent( signToken, identInfo, indexNum, signValue ) ;
    signNum.tokenStr = signValue ;
    signNum.type = CONSTANT ;
    operandStack.push( signNum ) ;
  } // else if
  else if ( calToken[i].type == CONSTANT ) {
    isConstant = true ;
    string tokenValue ;
    GetTokenValue( calToken[i], type, nextStr, caller ) ; // �����o��token����
  } // else if
  else if ( calToken[i].type == SIGN ) {
    isConstant = true ;
    stack<Token> signOperand ;
    stack<string> signOperator ;
    Sign( calToken, i, signOperand, signOperator, needType, caller ) ;
    GetTokenValue( signOperand.top(), type, nextStr, caller ) ;
  } // else if

  if ( type == "char" || type == "string" )
    nextStr = nextStr.substr( 1, nextStr.size()-2 ) ; // �h���Y�������޸��γ�޸�

  if ( isConstant ) { // �`��
    bool isFloat ;

    if ( IsNum( nextStr, isFloat ) ) { // sign�᭱���`�ƥu��O�Ʀr��true false�A����O��L�r��
      string num ;
      if ( signStr == "+" ) // �O�����A�i��������
        num = nextStr ;
      if ( signStr == "-" ) {
        if ( ! isFloat )
          num = NumToString( atoi( nextStr.c_str() ) * -1, false ) ;
        else {
          num = NumToString( atof( nextStr.c_str() ) * -1, true ) ;
        } // else

        // if ( nextStr[0] == '-' )
        //   num = nextStr.substr( 1, nextStr.size()-1 ); // �t�t�o���A�����}�Y���t��
        // else
        //   num = signStr + nextStr ;

      } // if
      else if ( signStr == "!" ) { // ��^stack�����G�n��true �� fasle
        num = NumToString( ! atof( nextStr.c_str() ), false ) ;
        NumToBool( num ) ;
      } // else if

      signNum.tokenStr = num ;
      signNum.type = CONSTANT ;
      operandStack.push( signNum ) ;
    } // if
    else { // sign�᭱���O���Ʀr���`��
          // throw new
    } // else
  } // if

} // C_Interpreter::Sign()

void C_Interpreter :: SignIdent( Token sign, Variable & identInfo, int indexNum, string & signValue ) {

  signValue = identInfo.value[indexNum] ;

  if ( sign.tokenStr == "+" || sign.tokenStr == "-" ) { // ���|��ident���O���~(���Oint ��float)�����D
    if ( sign.tokenStr == "-" ) {
      if ( identInfo.type == "int" )
        signValue = NumToString( atoi( signValue.c_str() ) * -1, false ) ;
      else
        signValue = NumToString( atof( signValue.c_str() ) * -1, true ) ;
    } // if
  } // if
  else if ( sign.tokenStr == "!" ) { // �u�i�Hint float bool
    signValue = NumToString( ! atof( signValue.c_str() ), false ) ;
    NumToBool( signValue ) ;
  } // else if

} // C_Interpreter::SignIdent()

void C_Interpreter :: NumToBool( string & value ) { // 0->false  1->true
  if ( value == "0" )
    value = "false" ;
  else
    value = "true" ;
} // C_Interpreter::NumToBool()

void C_Interpreter :: QuestionMark( vector<Token> calToken, int & i, stack<Token> & operandStack,
                                    stack<string> & operatorStack, string & needType, string caller ) {

  // '?' basic_expression ':' basic_expression
  // ���ݸ��u���v�����B�ⰵ���A���G����true ��false
  Operator( calToken[i].tokenStr, operandStack, operatorStack, needType, caller ) ;
  operatorStack.pop() ;  // pop "?"
  string boolAns = operandStack.top().tokenStr ;
  operandStack.pop() ; // pop "true" "false"
  vector<Token> trueExp, falseExp ;
  stack<Token> doOperand ;
  stack<string> doOperator ;
  string ans ;
  int endIndex ;

  i++ ;
  for ( ; i < calToken.size() && calToken[i].type != COLON ; i++ ) { // true�n���檺�b"?"�P":"����
    trueExp.push_back( calToken[i] ) ;
  } // for

  i++ ;  // ":" ���U�@��token�}�l�Ofalse�n����exp
  for ( ; i < calToken.size() ; i++ ) {
    falseExp.push_back( calToken[i] ) ;
  } // for

  vector<Token> doExp ;
  if ( boolAns == "false" ||  boolAns == "0" ) {
    doExp = falseExp ;
  } // if
  else {
    doExp = trueExp ;
  } // else if

  Evaluate_Basic_Expression( doExp, doOperand, doOperator, needType, caller ) ;
  operandStack.push( doOperand.top() ) ; // ���Gpush��B�⤸��stack

} // C_Interpreter::QuestionMark()


void C_Interpreter :: Operator( string currentOp, stack<Token> & operandStack, stack<string> & operatorStack,
                                string needType, string caller ) {    // �J��+ - * /

  if ( ! operatorStack.empty() ) {
    string topOp = operatorStack.top() ;

    while ( OperatorPriority( topOp )  >= OperatorPriority( currentOp ) ) {
      CalculateStack( operandStack, operatorStack, needType, caller ) ;

      if ( operatorStack.empty() ) {
        operatorStack.push( currentOp );
        return  ;
      } // if
      else
        topOp = operatorStack.top() ;

    } // while
  } // if

  operatorStack.push( currentOp );
  return ;
} // C_Interpreter::Operator()

int C_Interpreter :: OperatorPriority( string op ) { // �B��l�u���v

  if ( op == "*" || op == "/" || op == "%" )
    return 100 ;
  else if ( op == "+" || op == "-" )
    return 90 ;
  else if ( op == "<<" || op == ">>" )
    return 80 ;
  else if ( op == "<" ||  op == ">" ||  op == "<=" ||  op == ">=" )
    return 70 ;
  else if ( op == "==" || op == "!=" )
    return 60 ;
  else if ( op == "&&" )
    return 50 ;
  else if ( op == "||" )
    return 40 ;
  else if ( op == "?" )
    return 30 ;
  else if ( op == "=" || op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "%=" )
    return 20 ;
  else if ( op == "(" )  // �A��
    return 10 ;
  else
    return 0 ;

} // C_Interpreter::OperatorPriority()

// ���X�@�ӹB��l �� ��ӹB�⤸ ���B�� �A��B�⵲�G���stack
void C_Interpreter :: CalculateStack( stack<Token> & operandStack, stack<string> & operatorStack,
                                      string needType, string caller ) {


  stack<Token> c( operandStack ) ;
  while ( ! c.empty() ) {
    string temp = c.top().tokenStr ;
    c.pop() ;
  } // while


  string topOp = operatorStack.top() ;
  operatorStack.pop() ;
  Token token1 = operandStack.top() ; // run-time error
  operandStack.pop() ;
  Token token2 = operandStack.top() ;
  operandStack.pop() ;
  Token result ;
  // cout << "token1 : " << token1.tokenStr << "  token2 : " << token2.tokenStr << endl ;
  Calculate( topOp, token1, token2, result, needType, caller ) ;
  operandStack.push( result ) ;
  // cout << "result = " << result.tokenStr << endl ;
} // C_Interpreter::CalculateStack()

void C_Interpreter :: CalRestInStack( stack<Token> & operandStack, stack<string> & operatorStack,
                                      string needType, string caller ) {

  while ( ! operandStack.empty() && ! operatorStack.empty() ) {
    CalculateStack( operandStack, operatorStack, needType, caller ) ; // correct �O false �N�����H0
  } // while

} // C_Interpreter::CalRestInStack()

void C_Interpreter :: Calculate( string op, Token token1, Token token2, Token & result, string & needType,
                                 string caller ) {

  string type1, type2 ;
  string value1, value2 ;

  GetTokenValue( token1, type1, value1, caller ) ; // ���otoken1�ҥN���ȸ򫬺A

  GetTokenValue( token2, type2, value2, caller ) ; // ���otoken2�ҥN���ȸ򫬺A

  if ( type1 == "char" || type1 == "string" )
    value1 = value1.substr( 1, value1.size()-2 ) ; // �h���Y�������޸��γ�޸�
  if ( type2 == "char" || type2 == "string" )
    value2 = value2.substr( 1, value2.size()-2 ) ; // �h���Y�������޸��γ�޸�

  // cout << token1.tokenStr << " : " << value1 << ",  type : " << type1 << endl ;
  // cout << token2.tokenStr << " : " << value2 << ",  type : " << type2 << endl ;

  if ( op == "+" ) { // �[�k�i�H�Φb�r��μƦr�W

    // needType == "" �A cout << 5+8 --> �쥻���|�]�w�ݭn�ƻ򫬧O������
    if ( needType == "string" || ( needType == "" && type2 == "string" ) ) {
      needType = "string" ;
      result.tokenStr = "\"" + value2 + value1 + "\"" ;
      result.type = CONSTANT ;
    } // if
    else {
      bool isFloat1 = false, isFloat2 = false ;
      IsNum( value1, isFloat1 ) ; // �i��쥻�����O�Ostring�A�ݭn��int�٬Ofloat��
      IsNum( value2, isFloat2 ) ;
      if ( type1 == "float" || type2 == "float" || isFloat1 || isFloat2 ) // ���@�ӬOfloat�N��float��
        result.tokenStr = NumToString( atoi( value2.c_str() ) + atoi( value1.c_str() ), true ) ;
      else
        result.tokenStr = NumToString( atof( value2.c_str() ) + atof( value1.c_str() ), false ) ;

      result.type = CONSTANT ;
    } // else if

  } // if
  else if ( op == "=" ) { // token2 = token1
    Assign( token2, type2, type1, value1, result, caller ) ;
  } // else if
  else if ( op == "+=" ) { // token2 += token1 --> token2 = token2 + token1

    string assignValue ;
    if ( needType == "string" ) {
      assignValue = value2 + value1  ;
    } // if
    else if ( needType == "int" || needType == "float" ) {
      assignValue = NumToString( atof( value2.c_str() ) + atof( value1.c_str() ), true ) ;
    } // else if

    Assign( token2, type2, needType, assignValue, result, caller ) ; // �N��X��assignValue��token2
  } // else if
  else if ( op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" ) {

    bool numCompare = false ; // �n�r�����٬O�Ʀr���
    if ( ( type1 == "int" || type1 == "float" ) && ( type2 == "int" || type2 == "float" ) ) // ��ӳ��O�Ʀr�A�μƦr���
      numCompare = true ;

    if ( op == "<" ) {
      if ( ( ! numCompare && value2 < value1 ) ||
           ( numCompare && atof( value2.c_str() ) < atof( value1.c_str() ) ) ) {
        result.tokenStr = "true" ;
      } // if
      else
        result.tokenStr = "false" ;

      result.type = CONSTANT ;
    } // if
    else if ( op == ">" ) {
      if ( ( ! numCompare && value2 > value1 ) ||
           ( numCompare && atof( value2.c_str() ) > atof( value1.c_str() ) ) ) {
        result.tokenStr = "true" ;
      } // if
      else {
        result.tokenStr = "false" ;
      } // else

      result.type = CONSTANT ;
    } // else if
    else if ( op == "<=" ) {
      if ( ( ! numCompare && value2 <= value1 ) ||
           ( numCompare && atof( value2.c_str() ) <= atof( value1.c_str() ) ) ) {
        result.tokenStr = "true" ;
      } // if
      else {
        result.tokenStr = "false" ;
      } // else

      result.type = CONSTANT ;
    } // else if
    else if ( op == ">=" ) {
      if ( ( ! numCompare && value2 >= value1 ) ||
           ( numCompare && atof( value2.c_str() ) >= atof( value1.c_str() ) ) ) {
        result.tokenStr = "true" ;
      } // if
      else {
        result.tokenStr = "false" ;
      } // else

      result.type = CONSTANT ;
    } // else if
    else if ( op == "==" ) {
      if ( ( ! numCompare && value2 == value1 ) ||
           ( numCompare && atof( value2.c_str() ) == atof( value1.c_str() ) ) ) {
        result.tokenStr = "true" ;
      } // if
      else {
        result.tokenStr = "false" ;
      } // else

      result.type = CONSTANT ;
    } // else if
    else if ( op == "!=" ) {
      if ( ( ! numCompare && value2 != value1 ) ||
           ( numCompare && atof( value2.c_str() ) != atof( value1.c_str() ) ) ) {
        result.tokenStr = "true" ;
      } // if
      else {
        result.tokenStr = "false" ;
      } // else

      result.type = CONSTANT ;
    } // else if
  } // else if
  else if ( op == "&&" || op == "||" ) { // int float bool

    int ans ;
    if ( op == "&&" )
      ans = atoi( value2.c_str() ) && atoi( value1.c_str() ) ;
    else // op == "||"
      ans = atoi( value2.c_str() ) || atoi( value1.c_str() ) ;

    if ( ans == 1 )
      result.tokenStr = "true" ;
    else
      result.tokenStr = "false" ;

    result.type = CONSTANT ;
  } // else if
  else { // ��L�B��u��int float - * / % << >>

    // int is only compatible with float and string (and int itself).  string�i�H�� - * / �� int i = "5" - "3"
    bool isFloat1 = false, isFloat2 = false ;
    IsNum( value1, isFloat1 ) ; // �i��쥻�����O�Ostring�A�ݭn��int�٬Ofloat��
    IsNum( value2, isFloat2 ) ;

    if ( op == "-" ) {
      if ( type1 == "float" || type2 == "float" || isFloat1 || isFloat2 ) // ���@�ӬOfloat�N��float��
        result.tokenStr = NumToString( atof( value2.c_str() ) - atof( value1.c_str() ), true ) ;
      else
        result.tokenStr = NumToString( atoi( value2.c_str() ) - atoi( value1.c_str() ), false ) ;
      result.type = CONSTANT ;
    } // if
    else if ( op == "*" ) {
      if ( type1 == "float" || type2 == "float" || isFloat1 || isFloat2 ) // ���@�ӬOfloat�N��float��
        result.tokenStr = NumToString( atof( value2.c_str() ) * atof( value1.c_str() ), true ) ;
      else
        result.tokenStr = NumToString( atoi( value2.c_str() ) * atoi( value1.c_str() ), false ) ;
      result.type = CONSTANT ;
    } // else if
    else if ( op == "/" ) {
      if ( atof( value1.c_str() ) == 0 )
        throw new DividedByZero() ;
      if ( type1 == "float" || type2 == "float" || isFloat1 || isFloat2 ) // ���@�ӬOfloat�N��float��
        result.tokenStr = NumToString( atof( value2.c_str() ) / atof( value1.c_str() ), true ) ;
      else
        result.tokenStr = NumToString( atoi( value2.c_str() ) / atoi( value1.c_str() ), false ) ;
      result.type = CONSTANT ;
    } // else if
    else if ( op == "%" ) { // �u�� int % int�A�����D�|���|��float��error

      if ( atof( value1.c_str() ) == 0 ) {
        throw new DividedByZero() ;
      } // if

      result.tokenStr = NumToString( atoi( value2.c_str() ) % atoi( value1.c_str() ), false ) ;
      result.type = CONSTANT ;
    } // else if
    else if ( op == "-=" ) {
      string assignValue = NumToString( atof( value2.c_str() ) - atof( value1.c_str() ), true ) ;
      Assign( token2, type2, type1, assignValue, result, caller ) ;
    } // else if
    else if ( op == "*=" ) {
      string assignValue = NumToString( atof( value2.c_str() ) * atof( value1.c_str() ), true ) ;
      Assign( token2, type2, type1, assignValue, result, caller ) ;
    } // else if
    else if ( op == "/=" ) {
      if ( atof( value1.c_str() ) == 0 )
        throw DividedByZero() ;
      string assignValue = NumToString( atof( value2.c_str() ) / atof( value1.c_str() ), true ) ;
      Assign( token2, type2, type1, assignValue, result, caller ) ;
    } // else if
    else if ( op == "%=" ) { // �u�� int % int
      if ( atof( value1.c_str() ) == 0 )
        throw new DividedByZero() ;
      string assignValue = NumToString( atoi( value2.c_str() ) % atoi( value1.c_str() ), false ) ;
      Assign( token2, type2, type1, assignValue, result, caller ) ;
    } // else if
    else if ( op == ">>" || op == "<<" ) { // �u�� int << int

      if ( atof( value1.c_str() ) == 0 )
        throw new DividedByZero() ;  //  <<, >> ���k�䥲���O�D�sinteger�A�����D�O���ؿ�

      if ( op == ">>" )
        result.tokenStr = NumToString( atoi( value2.c_str() ) >> atoi( value1.c_str() ), false ) ;
      else // op == "<<"
        result.tokenStr = NumToString( atoi( value2.c_str() ) << atoi( value1.c_str() ), false ) ;
      result.type = CONSTANT ;
    } // else if


  } // else

} // C_Interpreter::Calculate()

// �o��token�N���ȸ򫬺A
void C_Interpreter :: GetTokenValue( Token token, string & type, string & value, string caller ) {

  if ( token.type == IDENT ) {
    Variable ident ;
    int index = 0 ;
    bool funcPara = false, byFuncRef = false, byRef = false ;
    string referFunc, referVar ;
    GetVariableInfo( token.tokenStr, ident, caller, index,
                     funcPara, byRef, byFuncRef, referFunc, referVar ) ;
    type = ident.type ;
    value = ident.value[index] ;
    // if ( type == "string" || type == "char" ) {
    //   value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸��γ�޸�->
    // } // if

  } // if
  else if ( token.type == IDENT_ARRAY ) {
    int leftParen = token.tokenStr.find( "[" ) ;
    int rightParen = token.tokenStr.size() - 1 ;
    string identName = token.tokenStr.substr( 0, leftParen ) ;
    int index = atoi( token.tokenStr.substr( leftParen+1, rightParen-leftParen-1 ).c_str() ) ; // ���o[]�����޼Ʀr
    Variable arrayVar ;
    int i ;
    bool funcPara = false, byFuncRef = false, byRef = false ;
    string referFunc, referVar ;
    GetVariableInfo( identName, arrayVar, caller, i, funcPara, byRef, byFuncRef, referFunc, referVar ) ;
    type = arrayVar.type ;
    value = arrayVar.value[index] ;
    // if ( type == "string" || type == "char" ) {
    //   value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸��γ�޸�
    // } // if
  } // else if
  else if ( token.type == CONSTANT ) {
    value = token.tokenStr ;
    if ( token.tokenStr[0] == '\'' ) {
      // value = value.substr( 1, value.size()-2 ) ; // �h���Y������޸�
      type = "char" ;
    } // if
    else if ( token.tokenStr[0] == '"' ) {
      // value = value.substr( 1, value.size()-2 ) ; // �h���Y�������޸�
      type = "string" ;
    } // else if
    else if ( token.tokenStr == "true" || token.tokenStr == "false" ) {
      type = "bool" ;
      if ( token.tokenStr == "true" )
        value = "1" ;
      else
        value = "0" ;
    } // else if
    else {
      bool isFloat = false ;
      IsNum( token.tokenStr, isFloat ) ;
      if ( ! isFloat )
        type = "int" ;
      else
        type = "float" ;
    } // else

  } // else if
} // C_Interpreter::GetTokenValue()

void C_Interpreter :: Assign( Token left, string leftType, string rightType, string assignValue,
                              Token & result, string caller ) {

  Variable var ;
  string fullName = left.tokenStr ;
  string identName = left.tokenStr ;
  int index = 0 ;
  bool funcPara = false, byFuncRef = false, byRef = false ;
  string referFunc, referVar ;

  if ( left.type == IDENT_ARRAY ) { // �p�G�Oarray�A�����o���ޭ�
    int leftParen = fullName.find( "[" ) ;
    int rightParen = fullName.size() - 1 ;
    identName = left.tokenStr.substr( 0, leftParen ) ;
    index = atoi( fullName.substr( leftParen+1, rightParen-leftParen-1 ).c_str() ) ; // ���o[]�����޼Ʀr
    int i ;
    GetVariableInfo( identName, var, caller, i,
                     funcPara, byRef, byFuncRef, referFunc, referVar ) ; // ��token2 �ܼƪ����
  } // if
  else
    GetVariableInfo( identName, var, caller, index,
                     funcPara, byRef, byFuncRef, referFunc, referVar ) ; // ��token2 �ܼƪ����

  // GetVariableInfo( identName, var, caller, index ) ; // ��token2 �ܼƪ����

  if ( leftType == "int" ) {
    int intNum = atoi( assignValue.c_str() ) ;
    var.value[index] = NumToString( intNum, false ) ;
  } // if
  else if ( leftType == "string" ) {
    var.value[index] = "\"" + assignValue + "\"" ;
  } // else if
  else if ( leftType == "char" ) {
    var.value[index] = "'" + assignValue + "'" ;
  } // else if
  else {
    var.value[index] = assignValue ;
  } // else

  UpdateVariable( identName, var, caller ) ; // ��stoken2�ܼƭ�
  if ( leftType == "bool" ) {
    if ( assignValue == "0" )
      result.tokenStr = "false" ;
    else
      result.tokenStr = "true" ;
  } // if
  else
    result.tokenStr = var.value[index] ;
  result.type = CONSTANT ;

} // C_Interpreter::Assign()


bool C_Interpreter :: IsNum( string str, bool & isFloat ) { // ��str�O���O��ƩάO�p��

  bool hasDot = false ;
  int i = 0 ;
  if ( str[0] == '-' ) // �t�ơA�q�t���᭱�}�l�ˬd
    i = 1 ;

  for ( ; i < str.size() ; i++ ) {

    if ( (  str[i] == '.' && hasDot ) || ( str[i] != '.' && ! IsDigit( str[i] ) ) ) {
      return false ;
    } // if
    else if ( str[i] == '.' && ! hasDot ) {
      hasDot = true ;
      isFloat = true ;
    } // else if

  } // for

  return true ;
} // C_Interpreter::IsNum()

void C_Interpreter :: GetVariableInfo( string identName, Variable & var, string caller, int & index,
                                        bool & funcParaOrDefine, bool & byRef, bool & byFuncRef,
                                        string & referFunc, string & referVar ) {

  index = 0 ;
  // project4

  if ( ! caller.empty() ) {  // �O�bfunctoin��-->�ݬO���Ofunction���Ѽ�
    // cout << "caller : " << caller << endl ;
    map<string, Function>::iterator iter ;
    iter = mFunctionTable.find( caller ) ;
    if ( iter != mFunctionTable.end() ) {
      for ( int i = 0 ; i < iter->second.parameterList.size() ; i++ ) {
        if ( iter->second.parameterList[i].name == identName ) {
          funcParaOrDefine = true ; // �Ocaller���Ѽ�
          if ( ! iter->second.parameterList[i].referenceFunction.empty() ) { // �ѦҨ�Y��function���Y�ӰѼ�
            byFuncRef = true ;
            referFunc = iter->second.parameterList[i].referenceFunction ;
            map<string, Function>::iterator funcIter ;
            funcIter = mFunctionTable.find( referFunc ) ;
            referVar = iter->second.parameterList[i].referenceVar ;
            for ( int j = 0 ; j < funcIter->second.parameterList.size() ; j++ ) {
              if ( funcIter->second.parameterList[j].name == referVar ) {
                var = funcIter->second.parameterList[j] ;
                return ;
              } // if
            } // for

            for ( int k = funcIter->second.funcDefineVar.size() - 1 ; k >= 0 ; k-- ) {
              map<string, Variable>::iterator varIter ;
              varIter = funcIter->second.funcDefineVar[k].find( referVar ) ;
              if ( varIter != funcIter->second.funcDefineVar[k].end() ) {
                var = varIter->second ;
                return ;
              } // if
            } // for
          } // if
          else if ( ! iter->second.parameterList[i].referenceVar.empty() ) { // �O�ѦҨ�O���ܼ�
            byRef = true ;
            map<string, Variable>::iterator varIter ;
            string referenceVar = iter->second.parameterList[i].referenceVar ;
            varIter = mVarTable[iter->second.parameterList[i].referenceLevel].find( referenceVar ) ;
            index = iter->second.parameterList[i].referenceIndex ;
            var = varIter->second ;
            return ;
          } // else if
          else { // function�ѼƬOcall by value
            // cout << identName << " not reference! " << endl ;
            var = iter->second.parameterList[i] ;
            return ;
          } // else

        } // if
      } // for


      for ( int i = iter->second.funcDefineVar.size() - 1 ; i >= 0 ; i-- ) {
        map<string, Variable>::iterator varIter ;
        varIter = iter->second.funcDefineVar[i].find( identName ) ;
        if ( varIter != iter->second.funcDefineVar[i].end() ) {
          funcParaOrDefine = true ;
          var = varIter->second ;
          return ;
        } // if
      } // for
    } // if
  } // if

  map<string, Variable>::iterator iter ;
  // ��ϰ��ܼƻP�����ܼƦ��S���@�˪�id�W��
  for ( int i = mVarTable.size() - 1 ; i >= 0 ; i-- ) {
    iter = mVarTable[i].find( identName ) ;
    if ( iter != mVarTable[i].end() ) {
      var = iter->second ;
      return ;
    } // if
  } // for

  return ;
} // C_Interpreter::GetVariableInfo()

void C_Interpreter :: FunctionDone( string caller ) {

  map<string, Function>::iterator iter ;
  iter = mFunctionTable.find( caller ) ;
  if ( iter != mFunctionTable.end() ) {
    for ( int i = 0 ; i < iter->second.parameterList.size() ; i++ ) {
      iter->second.parameterList[i].referenceVar = "" ;
      iter->second.parameterList[i].value.clear() ;
      iter->second.parameterList[i].referenceIndex = 0 ;
      iter->second.parameterList[i].referenceFunction = "" ;
    } // for
  } // if
} // C_Interpreter::FunctionDone()

void C_Interpreter :: UpdateVariable( string identName, Variable updateVar, string caller ) {

  // project4
  if ( ! caller.empty() ) {  // �O�bfunctoin��-->�ݬO���Ofunction���Ѽ�
    map<string, Function>::iterator iter ;
    iter = mFunctionTable.find( caller ) ;
    if ( iter != mFunctionTable.end() ) {
      for ( int i = 0 ; i < iter->second.parameterList.size() ; i++ ) {

        if ( iter->second.parameterList[i].name == identName ) {

          if ( ! iter->second.parameterList[i].referenceFunction.empty() ) {
            string referFunc = iter->second.parameterList[i].referenceFunction ;
            string referVar = iter->second.parameterList[i].referenceVar ;
            map<string, Function>::iterator funcIter ;
            funcIter = mFunctionTable.find( referFunc ) ;

            for ( int j = 0 ; j < funcIter->second.parameterList.size() ; j++ ) {
              if ( funcIter->second.parameterList[j].name == referVar ) {
                funcIter->second.parameterList[j] = updateVar ;
                return ;
              } // if
            } // for

            for ( int k = funcIter->second.funcDefineVar.size() - 1 ; k >= 0 ; k-- ) {
              map<string, Variable>::iterator varIter ;
              varIter = funcIter->second.funcDefineVar[k].find( referVar ) ;
              if ( varIter != funcIter->second.funcDefineVar[k].end() ) {
                varIter->second = updateVar ;
                return ;
              } // if
            } // for
          } // if
          else if ( ! iter->second.parameterList[i].referenceVar.empty() ) {
            map<string, Variable>::iterator varIter ;
            string referenceVar = iter->second.parameterList[i].referenceVar ;
            varIter = mVarTable[iter->second.parameterList[i].referenceLevel].find( referenceVar ) ;
            varIter->second = updateVar ;
            return ;
          } // if
          else {
            iter->second.parameterList[i] = updateVar ;
            return ;
          } // end else

        } // if
      } // for

      for ( int i = iter->second.funcDefineVar.size() - 1 ; i >= 0 ; i-- ) {
        map<string, Variable>::iterator varIter ;
        varIter = iter->second.funcDefineVar[i].find( identName ) ;
        if ( varIter != iter->second.funcDefineVar[i].end() ) {
          varIter->second = updateVar ;
          return ;
        } // if
      } // for

    } // if
  } // if

  map<string, Variable>::iterator iter ;
  for ( int i = mVarTable.size() -1 ; i >= 0 ; i-- ) {
    iter = mVarTable[i].find( identName ) ;
    if ( iter != mVarTable[i].end() ) {
      iter->second = updateVar ;
      return ;
    } // if
  } // for


} // C_Interpreter::UpdateVariable()


void C_Interpreter :: GetFunctionInfo( string identName, Function & func ) {

  map<string, Function>::iterator iter ;
  iter = mFunctionTable.find( identName ) ;
  func = iter->second ;

} // C_Interpreter::GetFunctionInfo()

void C_Interpreter :: UpdateFunction( string identName, Function updateFunc ) {

  map<string, Function>::iterator iter ;
  iter = mFunctionTable.find( identName ) ;
  iter->second = updateFunc ;

} // C_Interpreter::UpdateFunction()

