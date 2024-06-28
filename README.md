# SummerHackson

学校課題の2Dゲーム
今はC++で完全リニューアルしたいと思います。
元々のアーキテクチャを見るくらいなら死ねる気がしますんで、此度は完全にゼロから、ちゃんとアーキテクチャしてから作りたいと思います。

src/の中に入ってるのがC++で書き始めたソースファイルです。それ以外のは学校一年目でC言語がメインになるファイルです。

premakeについて：
  プロジェクトの設置は一通り出来ているんですが、preprocessor definationsの設定だけが変える方法がまだ調べています。
  preprocessor definationsの、「親またはプロジェクトの規定値を継承します」をoffにしていただけるとビルドされます。(UNICODEを定義せずに)
  Edit: DirectXのinclude/library pathのマクロ化がまだ些か問題があります。DirectX SDKインストールフォルダの問題でしょうか、調べる中。

My original DirectX 2D game for school work.
Currently just rewritting in C++ and also trying to architecture it "abit" better so it's not looks like the total garbege it does right now.

Done:  
  Abstracted window class, so the entry point can be less messy;
  //It's marginally better now only just after pulling out all the win32 stuff from it. Wish it was like this in the first place.
  Outline for the sprite2D system class;
  Started to do a wrapper for the directX math so I don't have to include it everywhere and also some functions so the code won't be a pile of mess whenever I try to do math;

To do:
  Code better;
  System manager so I can start to plan a better architecture instead of using this pile of doodoo;
  Implement object component system so I can decouple all the stuff and free myself from this stupid entangled mess;
  Many more...
