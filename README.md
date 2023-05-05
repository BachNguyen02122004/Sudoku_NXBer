# Probject game of NXBer
Game sử dụng thư viện SDL2 và ngôn ngữ C++ 


## GIỚI THIỆU
 Sudoku, ban đầu có tên gọi là Number Place là một trò chơi câu đố sắp xếp chữ số dựa trên logic theo tổ hợp. Mục tiêu của trò chơi là điền các chữ số vào một lưới 9×9 sao cho mỗi cột, mỗi hàng, và mỗi phần trong số chín lưới con 3×3 cấu tạo nên lưới chính đều chứa tất cả các chữ số từ 1 tới 9.
 
 ## Tổng quan
a) Các nguồn để hoàn thành và phát triển game 
--
 - Code tham khảo trên Lazyfoo và 1 số bài trên phattriengame123az
 - Music : Lấy từ các nguồn trên mạng
  
b) Luật chơi
--
 - Người chơi sử dụng chuột để di chuyển tới các ô (có thể sử dụng bàn phím) 
 - Sử dụng bàn phím để nhập số 
 - Bấm 's' hoặc 'p' để dừng nhạc
 - Bấm Hint trên màn hình để sử dụng gợi ý 
 - Nếu người chơi điền quá 3 ô thì sẽ thua cuộc 
 


c) Hướng dẫn chơi + đồ họa game 💯
--
- Cài đặt sdl2 và makefile vào vscode.
- Download source code về thư mục có chứa file sdl2 và vscode.
- Đưa hết tất các file .cpp, .h res ở trong thư mục chứa nó ra ngoài.
- Sau đó người chơi sử dụng make file để chạy chương trình.
* Người chơi bấm chọn Start để bắt đầu hoặc bấm quit để thoát trò chơi : 

![anh_menu1](https://user-images.githubusercontent.com/125589612/235361887-afdb204e-3fe8-4b5a-b906-f6ffb5f83ef6.png)
* Người chơi chọn Mức độ để tiến hành chơi game 

![anh_chedo1](https://user-images.githubusercontent.com/125589612/235335444-8839af7e-0582-41c6-91b6-f42f5be72ae1.png)
* sau khi chọn mức độ thì màn hình sẽ hiện ra bảng Sudoku, người chơi bắt đâu giải có tính time : 

![anh_game1](https://user-images.githubusercontent.com/125589612/235335454-5acee027-fb53-4ee3-aa42-aa43610649f6.png)
* Kết thúc game:

 ![youwin](https://user-images.githubusercontent.com/125589612/235683983-41e65b14-8831-4d18-a3b9-253a65c70d86.png)


 ![game_over2](https://user-images.githubusercontent.com/125589612/235335456-59a52d6a-f714-4984-a2aa-cb870317c667.png)




d) Âm thanh
--
 - Âm thanh sống động, phù hợp với trò chơi . 

e)Về cải tiến game :
--
 - Có thể thêm chức năng pause game vào chương trình. 
 - Cải tiến game sai quá 3 lần thì người chơi sẽ thua (tạo cho game có độ khó cao hơn) .
 
