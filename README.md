# OurC-Interpreter

## 開發平台
Windows 10

## 開發環境
Code::Block20.03

## 說明
- Our-C-Grammar-2016-07-15.doc裡有詳細的文法規則
- 此直譯器會根據使用者所輸入的文字，判斷是否符合所規定的C語言文法，若完全正確則會執行並輸出運算結果
- 支援全域及區域變數、陣列、巢狀條件判斷、迴圈、函數
- 提供以下幾種系統函數可以使用 :  
	| Function | Definition |
	| :--: | :--: |
	| ListAllVariables() | 列出全域變數，並按照變數名稱由小到大排序排序 |
	| ListAllFunctions() | 列出使用者所定義的函數，並按照函數名稱由小到大排序排 |
	| ListVariable( char name[] ) | 輸出指定變數的定義 |
	| ListFunction( char name[] ) | 輸出指定函數的定義 |
	| Done() | 程式結束執行 |

- 錯誤訊息分成以下幾種( 錯誤由上至下偵測 ) : 
	| Error Type | Error Message |
	| :--: | :--: | 
	| Lexical Error | Line 3 : unrecognized token with first char : '$' |
	| Syntactical Error | Line 2 : unexpected token : '`' |
	| Semantic Error | Line 5 : undefined identifier : 'bcd' |
	| Runtime Error | Runtime error : array 'a' index 'i' = 30 out of range! |

## 使用方法 :
1. 編譯並執行
2. 第一行須輸入一個數字(限整數)與enter作為test number使用
3. 開始輸入C語言的簡單程式碼，包含條件判斷、迴圈、函式等，此C語言直譯器會檢查語法是否有錯誤，若有錯誤則會輸出錯誤訊息，並讓使用者重新輸入，若完全正確，則會執行使用者所輸入的程式碼，並顯示結果

---
## 程式驗證結果
- 函數、迴圈、條件判斷部分
	![image](https://github.com/YunTing-Lee/OurC-Interpreter/blob/main/Picture/1.PNG)
- 偵測錯誤部分
	![image](https://github.com/YunTing-Lee/OurC-Interpreter/blob/main/Picture/2.PNG)
- 
放置在./TestData/ 內