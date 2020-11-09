# CHCenter
Next generation of host software to replace Uranus



QTranslation:

1. In program, all the strings to be translated need to follow syntax:

   ```
   tr( "Cannot open %1" ).arg( fileName )
   ```

   

2. In QT command line, go to project directory: 

   ```
   lupdate -pro CHCenter.pro -ts CHCenter_en_US.ts
   lupdate -pro CHCenter.pro -ts CHCenter_tc.ts
   lupdate -pro CHCenter.pro -ts CHCenter_cn.ts
   ```

3. use QT linguist to edit .ts file. while finished editing :
   ```
   lrelease CHCenter_en_US.ts
   lrelease CHCenter_tc.ts
   lrelease CHCenter_cn.ts
   ```
   
4. add .qm to qrc resources
   
   ```
   QTranslator t;
   t.load(":/CHCenter_en_US.ts");
   a.installTranslator(&t);
   ```