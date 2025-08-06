#pragma once

#include "Vector2D.h"
#include <map>
#include <string>
#include <vector>

struct MaterialParam
{
	std::string file_path;
	int all_num;
	int num_x;
	int num_y;
	int size_x;
	int size_y;
};

class ResourceManager
{
private:
	//自クラスのポインタ(アドレスの先にインスタンスを生成する)
	static ResourceManager* instance;

	//画像格納
	std::map<std::string, std::vector<int>> images_container;
	//音源格納
	std::map<std::string, int> sounds_container;

private:
	//コンストラクタをprivateにすることで、
	//自クラスのメンバ関数でインスタンスを生成できないようにする
	ResourceManager() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	ResourceManager(ResourceManager& v) = delete;
	ResourceManager& operator = (const ResourceManager& v) = delete;
public:
	~ResourceManager() = default;

public:
	/// <summary>
	/// インスタンスを取得する処理
	/// </summary>
	/// <returns>クラスのポインタ</returns>
	static ResourceManager* GetInstance();
	/// <summary>
	/// インスタンスを削除する処理
	/// </summary>
	static void DeleteInstance();

public:
	/**
	* 画像を取得する
	* @param file_name　ファイルパス
	* @param all_num	画像の総数
	* @param num_x		横の総数
	* @param num_y		縦の総数
	* @param size_x		横のサイズ(px)
	* @param size_y		縦のサイズ(px)
	* @return 読み込んだ画像ハンドルのstd::vector配列
	*/
	const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(MaterialParam element);

	/**
	* 音源を格納する
	* @param file_name  ファイルパス
	* @return   読み込んだ音源ハンドル(int型)
	*/
	const int& GetSounds(std::string file_name);
	const int& GetSounds(const char* file_name);

	//音源の再生
	static void rPlaySound(int _sound_path,int _play_type,int _reset = 1);

	/**
	* 全ての画像と音源を削除する
	*/
	void UnloadResourceAll();

private:
	/**
	* 画像ハンドルを読み込みリソースを作成する
	* @param file_name　ファイルパス
	*/
	void CreateImagesResource(std::string file_name);

	/**
	* 画像ハンドルを読み込みリソースを作成する
	* @param file_name　ファイルパス
	* @param all_num	画像の総数
	* @param num_x		横の総数
	* @param num_y		縦の総数
	* @param size_x		横のサイズ(px)
	* @param size_y		縦のサイズ(px)
	*/
	void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);

	/**
	* 音源ハンドルを読み込みリソースを作成する
	* @param file_name　ファイルパス
	*/
	void CreateSoundsResource(std::string file_name);
};



