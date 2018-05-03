#pragma once

inline void FillFishCfg(xx::MemPool* mp, PKG::CatchFish::Config_p& cfg)
{
	cfg.MPCreate(mp);
	cfg->fishCfgs.MPCreate(mp);

	// 捏造鱼的配置信息( 具体到每一帧的判定缺失中 )
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 0;
		fc->coin = 1;
		fc->name.MPCreate(mp, "angler");
		fc->frameCount = 120;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 55;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 1;
		fc->coin = 2;
		fc->name.MPCreate(mp, "bomb");
		fc->frameCount = 30;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 55;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 2;
		fc->coin = 3;
		fc->name.MPCreate(mp, "butterfly");
		fc->frameCount = 120;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 40;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 3;
		fc->coin = 4;
		fc->name.MPCreate(mp, "flat");
		fc->frameCount = 120;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 50;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 4;
		fc->coin = 5;
		fc->name.MPCreate(mp, "lobster");
		fc->frameCount = 50;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 50;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 5;
		fc->coin = 6;
		fc->name.MPCreate(mp, "nemo");
		fc->frameCount = 30;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 30;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 6;
		fc->coin = 7;
		fc->name.MPCreate(mp, "octopus");
		fc->frameCount = 60;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 50;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 7;
		fc->coin = 8;
		fc->name.MPCreate(mp, "sali");
		fc->frameCount = 119;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 50;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 8;
		fc->coin = 9;
		fc->name.MPCreate(mp, "seasnail");
		fc->frameCount = 101;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 50;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 9;
		fc->coin = 10;
		fc->name.MPCreate(mp, "seaturtle");
		fc->frameCount = 120;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 60;
		cfg->fishCfgs->Add(std::move(fc));
	}
	{
		auto fc = mp->MPCreate<PKG::CatchFish::FishConfig>();
		fc->typeId = 10;
		fc->coin = 11;
		fc->name.MPCreate(mp, "shark");
		fc->frameCount = 60;
		fc->collisionArea.MPCreate(mp);
		fc->collisionArea->radius = 80;
		cfg->fishCfgs->Add(std::move(fc));
	}

	// 填充子弹半径配置
	cfg->bulletRadius = 20;

	// 初始玩家金币数
	cfg->playerInitCoin = 10000;

}
