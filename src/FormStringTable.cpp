#include "FormStringTable.h"


FormStringTable* FormStringTable::GetSingleton()
{
	static FormStringTable singleton;
	return &singleton;
}


std::string_view FormStringTable::MapFormTypeToString(RE::FormType a_formType)
{
	auto it = _formTypeToStringMap.find(a_formType);
	if (it != _formTypeToStringMap.end()) {
		return it->second;
	} else {
		assert(false);
		return "EROR";
	}
}


RE::FormType FormStringTable::MapStringToFormType(const std::string_view& a_string)
{
	if (a_string.length() != 4) {
		return RE::FormType::None;
	}

	std::string str;
	str.reserve(4);
	for (auto ch : a_string) {
		str.push_back(static_cast<char>(std::toupper(ch)));
	}

	auto it = _stringToFormTypeMap.find(str);
	return it != _stringToFormTypeMap.end() ? it->second : RE::FormType::None;
}


FormStringTable::FormStringTable() :
	_formTypeToStringMap(),
	_stringToFormTypeMap()
{
	Insert(RE::FormType::None, "NONE");
	Insert(RE::FormType::PluginInfo, "TES4");
	Insert(RE::FormType::FormGroup, "GRUP");
	Insert(RE::FormType::GameSetting, "GMST");
	Insert(RE::FormType::Keyword, "KYWD");
	Insert(RE::FormType::LocationRefType, "LCRT");
	Insert(RE::FormType::Action, "AACT");
	Insert(RE::FormType::TextureSet, "TXST");
	Insert(RE::FormType::MenuIcon, "MICN");
	Insert(RE::FormType::Global, "GLOB");
	Insert(RE::FormType::Class, "CLAS");
	Insert(RE::FormType::Faction, "FACT");
	Insert(RE::FormType::HeadPart, "HDPT");
	Insert(RE::FormType::Eyes, "EYES");
	Insert(RE::FormType::Race, "RACE");
	Insert(RE::FormType::Sound, "SOUN");
	Insert(RE::FormType::AcousticSpace, "ASPC");
	Insert(RE::FormType::Skill, "SKIL");
	Insert(RE::FormType::MagicEffect, "MGEF");
	Insert(RE::FormType::Script, "SCPT");
	Insert(RE::FormType::LandTexture, "LTEX");
	Insert(RE::FormType::Enchantment, "ENCH");
	Insert(RE::FormType::Spell, "SPEL");
	Insert(RE::FormType::Scroll, "SCRL");
	Insert(RE::FormType::Activator, "ACTI");
	Insert(RE::FormType::TalkingActivator, "TACT");
	Insert(RE::FormType::Armor, "ARMO");
	Insert(RE::FormType::Book, "BOOK");
	Insert(RE::FormType::Container, "CONT");
	Insert(RE::FormType::Door, "DOOR");
	Insert(RE::FormType::Ingredient, "INGR");
	Insert(RE::FormType::Light, "LIGH");
	Insert(RE::FormType::Misc, "MISC");
	Insert(RE::FormType::Apparatus, "APPA");
	Insert(RE::FormType::Static, "STAT");
	Insert(RE::FormType::StaticCollection, "SCOL");
	Insert(RE::FormType::MovableStatic, "MSTT");
	Insert(RE::FormType::Grass, "GRAS");
	Insert(RE::FormType::Tree, "TREE");
	Insert(RE::FormType::Flora, "FLOR");
	Insert(RE::FormType::Furniture, "FURN");
	Insert(RE::FormType::Weapon, "WEAP");
	Insert(RE::FormType::Ammo, "AMMO");
	Insert(RE::FormType::NPC, "NPC_");
	Insert(RE::FormType::LeveledNPC, "LVLN");
	Insert(RE::FormType::KeyMaster, "KEYM");
	Insert(RE::FormType::AlchemyItem, "ALCH");
	Insert(RE::FormType::IdleMarker, "IDLM");
	Insert(RE::FormType::Note, "NOTE");
	Insert(RE::FormType::ConstructibleObject, "COBJ");
	Insert(RE::FormType::Projectile, "PROJ");
	Insert(RE::FormType::Hazard, "HAZD");
	Insert(RE::FormType::SoulGem, "SLGM");
	Insert(RE::FormType::LeveledItem, "LVLI");
	Insert(RE::FormType::Weather, "WTHR");
	Insert(RE::FormType::Climate, "CLMT");
	Insert(RE::FormType::ShaderParticleGeometryData, "SPGD");
	Insert(RE::FormType::ReferenceEffect, "RFCT");
	Insert(RE::FormType::Region, "REGN");
	Insert(RE::FormType::Navigation, "NAVI");
	Insert(RE::FormType::Cell, "CELL");
	Insert(RE::FormType::Reference, "REFR");
	Insert(RE::FormType::ActorCharacter, "ACHR");
	Insert(RE::FormType::ProjectileMissile, "PMIS");
	Insert(RE::FormType::ProjectileArrow, "PARW");
	Insert(RE::FormType::ProjectileGrenade, "PGRE");
	Insert(RE::FormType::ProjectileBeam, "PBEA");
	Insert(RE::FormType::ProjectileFlame, "PFLA");
	Insert(RE::FormType::ProjectileCone, "PCON");
	Insert(RE::FormType::ProjectileBarrier, "PBAR");
	Insert(RE::FormType::PlacedHazard, "PHZD");
	Insert(RE::FormType::WorldSpace, "WRLD");
	Insert(RE::FormType::Land, "LAND");
	Insert(RE::FormType::NavMesh, "NAVM");
	Insert(RE::FormType::TLOD, "TLOD");
	Insert(RE::FormType::Dialogue, "DIAL");
	Insert(RE::FormType::Info, "INFO");
	Insert(RE::FormType::Quest, "QUST");
	Insert(RE::FormType::Idle, "IDLE");
	Insert(RE::FormType::Package, "PACK");
	Insert(RE::FormType::CombatStyle, "CSTY");
	Insert(RE::FormType::LoadScreen, "LSCR");
	Insert(RE::FormType::LeveledSpell, "LVSP");
	Insert(RE::FormType::AnimatedObject, "ANIO");
	Insert(RE::FormType::Water, "WATR");
	Insert(RE::FormType::EffectShader, "EFSH");
	Insert(RE::FormType::TOFT, "TOFT");
	Insert(RE::FormType::Explosion, "EXPL");
	Insert(RE::FormType::Debris, "DEBR");
	Insert(RE::FormType::ImageSpace, "IMGS");
	Insert(RE::FormType::ImageAdapter, "IMAD");
	Insert(RE::FormType::FormList, "FLST");
	Insert(RE::FormType::Perk, "PERK");
	Insert(RE::FormType::BodyPartData, "BPTD");
	Insert(RE::FormType::AddonNode, "ADDN");
	Insert(RE::FormType::ActorValueInfo, "AVIF");
	Insert(RE::FormType::CameraShot, "CAMS");
	Insert(RE::FormType::CameraPath, "CPTH");
	Insert(RE::FormType::VoiceType, "VTYP");
	Insert(RE::FormType::MaterialType, "MATT");
	Insert(RE::FormType::Impact, "IPCT");
	Insert(RE::FormType::ImpactDataSet, "IPDS");
	Insert(RE::FormType::Armature, "ARMA");
	Insert(RE::FormType::EncounterZone, "ECZN");
	Insert(RE::FormType::Location, "LCTN");
	Insert(RE::FormType::Message, "MESG");
	Insert(RE::FormType::Ragdoll, "RGDL");
	Insert(RE::FormType::DefaultObject, "DOBJ");
	Insert(RE::FormType::LightingMaster, "LGTM");
	Insert(RE::FormType::MusicType, "MUSC");
	Insert(RE::FormType::Footstep, "FSTP");
	Insert(RE::FormType::FootstepSet, "FSTS");
	Insert(RE::FormType::StoryManagerBranchNode, "SMBN");
	Insert(RE::FormType::StoryManagerQuestNode, "SMQN");
	Insert(RE::FormType::StoryManagerEventNode, "SMEN");
	Insert(RE::FormType::DialogueBranch, "DLBR");
	Insert(RE::FormType::MusicTrack, "MUST");
	Insert(RE::FormType::DialogueView, "DLVW");
	Insert(RE::FormType::WordOfPower, "WOOP");
	Insert(RE::FormType::Shout, "SHOU");
	Insert(RE::FormType::EquipSlot, "EQUP");
	Insert(RE::FormType::Relationship, "RELA");
	Insert(RE::FormType::Scene, "SCEN");
	Insert(RE::FormType::AssociationType, "ASTP");
	Insert(RE::FormType::Outfit, "OTFT");
	Insert(RE::FormType::ArtObject, "ARTO");
	Insert(RE::FormType::MaterialObject, "MATO");
	Insert(RE::FormType::MovementType, "MOVT");
	Insert(RE::FormType::SoundRecord, "SNDR");
	Insert(RE::FormType::DualCastData, "DUAL");
	Insert(RE::FormType::SoundCategory, "SNCT");
	Insert(RE::FormType::SoundOutputModel, "SOPM");
	Insert(RE::FormType::CollisionLayer, "COLL");
	Insert(RE::FormType::ColorForm, "CLFM");
	Insert(RE::FormType::ReverbParam, "REVB");
	Insert(RE::FormType::LensFlare, "LENS");
	Insert(RE::FormType::LensSprite, "LSPR");
	Insert(RE::FormType::VolumetricLighting, "VOLI");

	if (_formTypeToStringMap.size() != to_underlying(RE::FormType::Max)) {
		for (auto type = RE::FormType::None; type < RE::FormType::Max; ++type) {
			if (_formTypeToStringMap.find(type) == _formTypeToStringMap.end()) {
				_FATALERROR("Failed to insert %02X", to_underlying(type));
			}
		}
		assert(false);
	}
}


void FormStringTable::Insert(RE::FormType a_formType, const char* a_string)
{
	auto formTypeToStringResult = _formTypeToStringMap.insert(std::make_pair(a_formType, a_string));
	assert(formTypeToStringResult.second);

	auto stringToFormTypeResult = _stringToFormTypeMap.insert(std::make_pair(a_string, a_formType));
	assert(stringToFormTypeResult.second);
}
