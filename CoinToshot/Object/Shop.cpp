#include "../Utility/InputPad.h"
#include "Shop.h"
#include "../Scene/InGameScene.h"
#include "ObjectManager.h"

Shop::Shop(InGameScene* _ingame)
{
	camera = Camera::Get();
	ingame = _ingame;

	shop_flg = false;
	shop_cd = SHOP_COOLDOWN;

	shop_cursor = 0;
	for (int i = 0; i < ITEM_NUM; i++)
	{
		buy_count[i] = 0;
	}

	CreateItemImage();

	ResourceManager* rm = ResourceManager::GetInstance();
	shop_image = rm->GetImages("Resource/Images/Shop.png");

	button_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	back_se = rm->GetSounds("Resource/Sounds/Hanahana/button.mp3");
}

Shop::~Shop()
{

}

void Shop::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Shop::Finalize()
{

}

void Shop::Update()
{
	//ショップ展開時の処理
	if (shop_flg)
	{
		for (int i = 2; i < ITEM_NUM; i++)
		{
			//UserDataの所持中の弾を参照し、持っている弾を購入済みとする
			if (std::find(UserData::get_bullet.begin(), UserData::get_bullet.end(), i) != UserData::get_bullet.end())
			{
				buy_count[i] = 1;
			}
		}
		//カーソル移動処理
		if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::GetPressedButton(L_STICK_RIGHT))
		{
			if (++shop_cursor >= ITEM_NUM)
			{
				shop_cursor = 0;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) || InputPad::GetPressedButton(L_STICK_LEFT))
		{
			if (--shop_cursor < 0)
			{
				shop_cursor = ITEM_NUM - 1;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		//購入処理
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			BuyItem(shop_cursor);
		}
		//ショップを終わる
		if (InputPad::OnButton(XINPUT_BUTTON_B))
		{
			SetShop(false);
		}
	}
	//ショップ非展開時の処理
	else
	{
		if (shop_cd < SHOP_COOLDOWN)
		{
			shop_cd++;
		}
	}
}

void Shop::Draw()const
{
	__super::Draw();
	//ショップ本体描画
	//DrawBoxAA(local_location.x - (box_size.x/2),
	//		  local_location.y - (box_size.y/2),
	//		  local_location.x + (box_size.x/2),
	//		  local_location.y + (box_size.y/2),
	//	0x000000, false);
	SetFontSize(24);
	DrawRotaGraphF(local_location.x, local_location.y, 0.2f, 0, shop_image[0], true);
	//クールダウン描画
	if (shop_cd < SHOP_COOLDOWN)
	{
		DrawFormatStringF(local_location.x, local_location.y-30, 0xffffff, "%d", ((SHOP_COOLDOWN - shop_cd) / 60) + 1);
	}
	int span = 200;
	Vector2D shop_loc = { 200,400 };
	//ショップUI描画
	if (shop_flg)
	{
		DrawString(shop_loc.x, shop_loc.y - 100, "購入画面", 0xffffff);
		for (int i = 0; i < ITEM_NUM; i++)
		{
			DrawGraph(shop_loc.x + (i * span), shop_loc.y, item_image[i][shop_cursor != i], false);
			if (buy_count[i] >= item_list[i].buy_max)
			{
				DrawString(shop_loc.x + 70 + (i * span), shop_loc.y+150, "在庫なし", 0xff0000);
			}
			else if (UserData::coin < item_list[i].price)
			{
				DrawString(shop_loc.x + (i * span), shop_loc.y + 150, "コイン不足!", 0xff0000);
			}
			else
			{
				DrawFormatStringF(shop_loc.x + (i * span)+30, shop_loc.y + 150, 0x00ff00, "あと%dコ", item_list[i].buy_max - buy_count[i]);
			}
		}
	}
}

void Shop::Hit(ObjectBase* hit_object)
{
	//プレイヤーに触れたらショップ展開
	if (shop_cd >= SHOP_COOLDOWN && hit_object->GetObjectType() == ePLAYER)
	{
		SetShop(true);
	}
}

void Shop::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{

}

void Shop::CreateItemImage()
{
	int span = 200;
	for (int i = 0; i < ITEM_NUM; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			item_image[i][j] = MakeScreen(SHOP_ITEM_WIDTH, SHOP_ITEM_HEIGHT);
			int color = j == 0 ? item_list[i].text_color : 0xaaaaaa;
			SetDrawScreen(item_image[i][j]);
			ClearDrawScreen();
			DrawBoxAA(0, 0, SHOP_ITEM_WIDTH, SHOP_ITEM_HEIGHT, color == 0x000000 ? 0xffffff : 0x000000, true);
			DrawBoxAA(0, 0, SHOP_ITEM_WIDTH, SHOP_ITEM_HEIGHT, color, false);
			SetFontSize(48);
			DrawFormatString((span / 2) - (GetDrawStringWidth(item_list[i].name, strlen(item_list[i].name)) / 2), 20, color, "%s", item_list[i].name);
			SetFontSize(24);
			DrawFormatString((span / 2) - (GetDrawStringWidth(item_list[i].sub_text, strlen(item_list[i].sub_text)) / 2), 80, color, "%s", item_list[i].sub_text);
			UserData::DrawCoin({ (span / 2) - (GetDrawFormatStringWidth("   × %d",item_list[i].price) / 2) , 135 }, 12);
			DrawFormatString((span / 2) - (GetDrawFormatStringWidth(" × %d", item_list[i].price) / 2), 120, 0xffff00, " × %d", item_list[i].price);
			SetDrawScreen(DX_SCREEN_BACK);
		}
	}
}

void Shop::BuyItem(int cursor)
{
	Vector2D rand = 0;
	Vector2D rand_velocity = 0;
	//購入できるだけのコインがあるか
	if (UserData::coin >= item_list[cursor].price && buy_count[cursor] < item_list[cursor].buy_max)
	{
		switch (cursor)
		{
		case 0:	//回復アイテム
			rand = GetRandLoc();
			rand_velocity = { (camera->player_location.x - rand.x) / 10  ,
							  (camera->player_location.y - rand.y) / 10 };
			manager->CreateObject(eHEAL, rand, { 40,40 }, 20.f, rand_velocity);
			break;
		case 1: //磁石
			rand = GetRandLoc();
			rand_velocity = { (camera->player_location.x - rand.x) / 10  ,
							  (camera->player_location.y - rand.y) / 10 };
			manager->CreateObject(eMAGNET, rand, { 40,40 }, 20.f, rand_velocity);
			break;
		case 2: //強化弾
			UserData::get_bullet.push_back(2);
			break;
		case 3: //爆発弾
			UserData::get_bullet.push_back(3);
			break;
		case 4: //最強弾
			UserData::get_bullet.push_back(4);
			break;
		}
		//代金を引く
		UserData::coin -= item_list[cursor].price;
		//買ったアイテムのカウントを進める
		buy_count[cursor] += 1;
		//購入完了SE
		ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
		//ショップ終了
		SetShop(false);
	}
	//コインが無いなら失敗
	else
	{
		ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
	}

}

Vector2D Shop::GetRandLoc()
{
	Vector2D ret;

	//左右の端か上下の端かランダムで決定
	if ((bool)GetRand(1))
	{
		//左右の端
		ret.x = (float)(camera->GetCameraLocation().x - (SCREEN_WIDTH / 2) + (SCREEN_WIDTH * 2 * GetRand(1)));
		ret.y = (float)(camera->GetCameraLocation().y - (SCREEN_HEIGHT / 2) + GetRand(SCREEN_HEIGHT * 2));
	}
	else
	{
		//上下の端
		ret.x = (float)(camera->GetCameraLocation().x - (SCREEN_WIDTH / 2) + GetRand(SCREEN_WIDTH * 2));
		ret.y = (float)(camera->GetCameraLocation().y - (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT * 2 * GetRand(1)));
	}
	return ret;
}

void Shop::SetShop(bool _flg)
{
	//開始
	if (_flg)
	{
		ingame->SetShopFlg(true, this);
		shop_flg = true;
	}
	//終了
	else
	{
		ingame->SetShopFlg(false);
		shop_flg = false;
		shop_cd = 0;
	}
}