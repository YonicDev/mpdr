#include <doormapicon.h>

#include "doorexcluder.h"
#include "ui_doorexcluder.h"
#include <QDebug>

DoorExcluder::DoorExcluder(QWidget *parent, Preset *main_preset)
    : QDialog(parent)
    , ui(new Ui::DoorExcluder)
{
    ui->setupUi(this);

    preset = main_preset;
    excluded_doors = vector<RoomDoorList>(preset->excluded_doors);

    QString mapimg_path = QCoreApplication::applicationDirPath().append("/resources/maps/");
    ui->label_DoorError->setStyleSheet("QLabel { color:transparent; }");

    initializeMap(ui->map_tallon,QPixmap(mapimg_path + "tallon_map.jpg"));
    initializeMap(ui->map_chozo,QPixmap(mapimg_path + "chozo_map.jpg"));
    initializeMap(ui->map_magmoor,QPixmap(mapimg_path + "magmoor_map.jpg"));
    initializeMap(ui->map_phendrana,QPixmap(mapimg_path + "phendrana_map.jpg"));
    initializeMap(ui->map_mines,QPixmap(mapimg_path + "mines_map.jpg"));

    addIcons(ui->map_tallon,TallonOverworld);
    addIcons(ui->map_chozo,ChozoRuins);
    addIcons(ui->map_magmoor,MagmoorCaverns);
    addIcons(ui->map_phendrana,PhendranaDrifts);
    addIcons(ui->map_mines,PhazonMines);
}

DoorExcluder::~DoorExcluder()
{
    delete selected_icon;
    delete ui;
}

void DoorExcluder::showEvent(QShowEvent *) {
    parentWidget()->setCursor(Qt::ArrowCursor);
}

void DoorExcluder::initializeMap(QGraphicsView *map, QPixmap image) {
    QGraphicsScene *scene = new QGraphicsScene();

    QImage bground(50, 50, QImage::Format_RGB888);
    for (int y = 0; y < 25; y++)
    {
        for (int x = 0; x < 25; x++)
        {
            bground.setPixel(x, y, qRgb(0x08, 0x15, 0x1D));
            bground.setPixel(x + 25, y, qRgb(0x20, 0x30, 0x39));
            bground.setPixel(x, y + 25, qRgb(0x20, 0x30, 0x39));
            bground.setPixel(x + 25, y + 25, qRgb(0x08, 0x15, 0x1D));
        }
    }

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(image);
    scene->setSceneRect(image.rect());
    scene->addPixmap(image);

    map->setBackgroundBrush(bground);
    map->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    map->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    map->setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
    map->setScene(scene);
}

void DoorExcluder::loadDoorDetails(DoorMapIcon *icon) {
    if(selected_icon!=nullptr)
        selected_icon->select(false);
    selected_icon = icon;
    selected_icon->select();
    ui->spinBox->setValue(icon->doorID());
    ui->lineEdit->setText(selected_icon->areaName());
    Qt::CheckState checked = selected_icon->isExcluded()?Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    ui->checkBox->setCheckState(checked);
    ui->checkBox->setEnabled(selected_icon->canBeExcluded());
    QString show_warn = selected_icon->canBeExcluded()?"transparent":"red";
    ui->label_DoorError->setStyleSheet(QString("QLabel { color:%1; }").arg(show_warn));
}

void DoorExcluder::on_checkBox_clicked(bool checked)
{
    if(selected_icon!=nullptr)
        selected_icon->exclude(checked);
}

void DoorExcluder::addIcons(QGraphicsView *map, World world) {
    RoomPointsList points;


    if(world == TallonOverworld) {

        const QPoint Landing_Canyon(618,646);
        const QPoint Landing_Gully(618,582);
        const QPoint Landing_Alcove(728,570);
        const QPoint Landing_Waterfall(788,598);
        const QPoint Landing_Temple(653,751);
        const QPoint Gully_Cannon(520,615);
        const QPoint Canyon_Cavern(533,704);
        const QPoint Temple_Monitor(653,834);
        const QPoint Crash_Waterfall(864,483);
        const QPoint Canyon_ChozoHall(436,578);
        const QPoint Canyon_Roots(402,691);
        const QPoint Monitor_TempleHall(670,985);
        const QPoint Crash_Entrance(1114,458);
        const QPoint Crash_Elevator(1072,330);
        const QPoint Chozo_CanyonHall(436,495);
        const QPoint Root_Hall(284,691);
        const QPoint Temple_Lobby(670,1069);
        const QPoint Crash_VenC(1185,451);
        const QPoint Overgrown_Tree(1072,248);
        const QPoint Overgrown_Elevator(1143,168);
        const QPoint Root_MagmoorHall(241,746);
        const QPoint MagmoorHall_Elevator(204,902);
        const QPoint Root_Arbor(180,691);
        const QPoint VenC_VenB(1278,435);
        const QPoint VenB_VenB(1278,472);
        const QPoint VenB_VenA(1277,523);
        const QPoint VenA_Core(1377,598);
        const QPoint ReactorCore_Access(1258,657);
        const QPoint ReactorAccess_Lift(1215,657);
        const QPoint ReactorAccess_Save(1236,634);
        const QPoint CargoLift_Hall(1029,701);
        const QPoint Biohazard_Transit(1029,773);
        const QPoint Biohazard_Security(1060,801);
        const QPoint Biotech_Security(1099,836);
        const QPoint Biotech_Conduit(1152,937);
        const QPoint Beta_Conduit(1090,970);
        const QPoint Beta_Hydro(1071,999);
        const QPoint Tree_Frigate(1197,1022);
        const QPoint Tree_Chamber(1295,997);
        const QPoint Tree_ElevatorD(1239,951);
        const QPoint Tree_Grove(1258,1058);
        const QPoint Tree_ElevatorE(1264,1044);
        const QPoint Tree_Chozo(1259,876);
        const QPoint Tree_Phazon(1220,1200);
        const QPoint Grove_Tunnel(1257,1174);

        points["Landing Site"] = {
            DoorPoint(Landing_Gully,Right),
            DoorPoint(Landing_Canyon,Right),
            DoorPoint(Landing_Temple,Up),
            DoorPoint(Landing_Alcove,Down),
            DoorPoint(Landing_Waterfall,Left)
        };
        points["Gully"] = {
            DoorPoint(Gully_Cannon,Right),
            DoorPoint(Landing_Gully,Left)
        };
        points["Canyon Cavern"] = {
            DoorPoint(Canyon_Cavern,Right),
            DoorPoint(Landing_Canyon,Left)
        };
        points["Temple Hall"] = {
            DoorPoint(Temple_Monitor,Up),
            DoorPoint(Landing_Temple,Down)
        };
        points["Alcove"] = {
            DoorPoint(Landing_Alcove,Up)
        };
        points["Waterfall Cavern"] = {
            DoorPoint(Landing_Waterfall,Right),
            DoorPoint(Crash_Waterfall,Left)
        };
        points["Tallon Canyon"] = {
            DoorPoint(Canyon_Cavern,Left),
            DoorPoint(Canyon_ChozoHall,Down),
            DoorPoint(Gully_Cannon,Left),
            DoorPoint(Canyon_Roots,Right)
        };
        points["Temple Security Station"] = {
            DoorPoint(Temple_Monitor,Down),
            DoorPoint(Monitor_TempleHall,Up)
        };
        points["Frigate Crash Site"] = {
            DoorPoint(Crash_Waterfall,Right),
            DoorPoint(Crash_Entrance,Left),
            DoorPoint(Crash_Elevator,Down)
        };
        points["Transport Tunnel A"] = {
            DoorPoint(Chozo_CanyonHall,Down),
            DoorPoint(Canyon_ChozoHall,Up)
        };
        points["Root Tunnel"] = {
            DoorPoint(Root_Hall,Right),
            DoorPoint(Canyon_Roots,Left)
        };
        points["Temple Lobby"] = {
            DoorPoint(Temple_Lobby,Up),
            DoorPoint(Monitor_TempleHall,Down)
        };
        points["Frigate Access Tunnel"] = {
            DoorPoint(Crash_VenC,Left),
            DoorPoint(Crash_Entrance,Right)
        };
        points["Overgrown Cavern"] = {
            DoorPoint(Crash_Elevator,Up),
            DoorPoint(Overgrown_Tree,Down)
        };
        points["Transport to Chozo Ruins West"] = {
            DoorPoint(Chozo_CanyonHall,Up)
        };
        points["Root Cave"] = {
            DoorPoint(Root_MagmoorHall,Up),
            DoorPoint(Root_Hall,Left),
            DoorPoint(Root_Arbor,Right)
        };
        points["Artifact Temple"] = {
            DoorPoint(Temple_Lobby,Down)
        };
        points["Main Ventilation Shaft Section C"] = {
            DoorPoint(VenC_VenB,Up),
            DoorPoint(Crash_VenC,Right)
        };
        points["Main Ventilation Shaft Section B"] = {
            DoorPoint(VenB_VenA,Up),
            DoorPoint(VenC_VenB,Down),
            DoorPoint(VenB_VenB,Up),
            DoorPoint(VenB_VenB,Down)
        };
        points["Main Ventilation Shaft Section A"] = {
            DoorPoint(VenB_VenA,Down),
            DoorPoint(VenA_Core,Up)
        };
        points["Transport Tunnel C"] = {
            DoorPoint(Overgrown_Tree,Up),
            DoorPoint(Overgrown_Elevator,Left)
        };
        points["Transport Tunnel B"] = {
            DoorPoint(Root_MagmoorHall,Down),
            DoorPoint(MagmoorHall_Elevator,Right)
        };
        points["Arbor Chamber"] = {
            DoorPoint(Root_Arbor,Left)
        };
        points["Transport to Chozo Ruins East"] = {
            DoorPoint(Overgrown_Elevator,Right)
        };
        points["Transport to Magmoor Caverns East"] = {
            DoorPoint(MagmoorHall_Elevator,Left)
        };
        points["Reactor Core"] = {
            DoorPoint(ReactorCore_Access,Right),
            DoorPoint(VenA_Core,Down)
        };
        points["Reactor Access"] = {
            DoorPoint(ReactorAccess_Lift,Right),
            DoorPoint(ReactorCore_Access,Left),
            DoorPoint(ReactorAccess_Save,Down),
        };
        points["Cargo Freight Lift to Deck Gamma"] = {
            DoorPoint(ReactorAccess_Lift,Left),
            DoorPoint(CargoLift_Hall,Up)
        };
        points["Savestation"] = {
            DoorPoint(ReactorAccess_Save,Up)
        };
        points["Deck Beta Transit Hall"] = {
            DoorPoint(CargoLift_Hall,Down),
            DoorPoint(Biohazard_Transit,Up)
        };
        points["Biohazard Containment"] = {
            DoorPoint(Biohazard_Transit,Down),
            DoorPoint(Biohazard_Security,Left)
        };
        points["Deck Beta Security Hall"] = {
            DoorPoint(Biohazard_Security,Right),
            DoorPoint(Biotech_Security,Up)
        };
        points["Biotech Research Area 1"] = {
            DoorPoint(Biotech_Security,Down),
            DoorPoint(Biotech_Conduit,Up)
        };
        points["Deck Beta Conduit Hall"] = {
            DoorPoint(Beta_Conduit,Right),
            DoorPoint(Biotech_Conduit,Down)
        };
        points["Connection Elevator to Deck Beta"] = {
            DoorPoint(Beta_Conduit,Left),
            DoorPoint(Beta_Hydro,Up)
        };
        points["Hydro Access Tunnel"] = {
            DoorPoint(Tree_Frigate,Left),
            DoorPoint(Beta_Hydro,Down)
        };
        points["Great Tree Hall"] = {
            DoorPoint(Tree_Frigate,Right),
            DoorPoint(Tree_Chamber,Left),
            DoorPoint(Tree_ElevatorD,Down),
            DoorPoint(Tree_Grove,Up),
            DoorPoint(Tree_ElevatorE,Up)
        };
        points["Great Tree Chamber"] = {
            DoorPoint(Tree_Chamber,Right)
        };
        points["Transport Tunnel D"] = {
            DoorPoint(Tree_Chozo,Left),
            DoorPoint(Tree_ElevatorD,Up)
        };
        points["Life Grove Tunnel"] = {
            DoorPoint(Tree_Grove,Down),
            DoorPoint(Grove_Tunnel,Up)
        };
        points["Transport Tunnel E"] = {
            DoorPoint(Tree_Phazon,Right),
            DoorPoint(Tree_ElevatorE,Down)
        };
        points["Transport to Chozo Ruins South"] = {
            DoorPoint(Tree_Chozo,Right)
        };
        points["Life Grove"] = {
            DoorPoint(Grove_Tunnel,Down)
        };
        points["Transport to Phazon Mines East"] = {
            DoorPoint(Tree_Phazon,Left)
        };
    } else if (world == ChozoRuins) {

        const QPoint Tallon_Entrance(506,1096);
        const QPoint Plaza_Entrance(524,956);
        const QPoint Plaza_FountainHall(581,858);
        const QPoint Plaza_ShrineHall(475,843);
        const QPoint Plaza_NurseryHall(480,723);
        const QPoint Plaza_VaultHall(619,683);
        const QPoint Plaza_PistonHall(570,969);
        const QPoint Fountain_PlazaHall(721,899);
        const QPoint Shrine_PlazaHall(313,843);
        const QPoint Eyon_PlazaHall(404,680);
        const QPoint Vault_PlazaHall(666,587);
        const QPoint Fountain_Arboretum(775,959);
        const QPoint Fountain_Meditation(751,1034);
        const QPoint Shrine_TowerHall(185,843);
        const QPoint Eyon_Nursery(321,623);
        const QPoint Vault_ElevatorHall(666,438);
        const QPoint Training_MagmaHall(565,1194);
        const QPoint Training_PistonHall(565,1053);
        const QPoint Arboretum_Fountain(830,1044);
        const QPoint Magma_MeditationHall(681,1113);
        const QPoint LightTower_ShrineHall(99,843);
        const QPoint Nursery_SaveStation(241,614);
        const QPoint Nursery_NorthAtrium(276,543);
        const QPoint Magmoor_VaultHall(666,236);
        const QPoint Magma_TrainingHall(624,1257);
        const QPoint Arboretum_SunLobby(857,933);
        const QPoint Arboretum_Gathering(961,1044);
        const QPoint LightTower_Chamber(99,1018);
        const QPoint Gallery_NorthAtrium(347,486);
        const QPoint Magmoor_SunTower(707,191);
        const QPoint Magmoor_TotemHall(635,191);
        const QPoint SunLobby_SunAccess(849,818);
        const QPoint Gathering_ArboretumHall(1048,1054);
        const QPoint Gallery_TotemHall(346,393);
        const QPoint Gallery_Map(321,463);
        const QPoint SunTower_SunChamber(819,191);
        const QPoint Totem_MagmoorHall(517,207);
        const QPoint Sunchamber_Access(849,700);
        const QPoint Gathering_WateryHall(1093,991);
        const QPoint Gathering_SaveStation(1081,1143);
        const QPoint Gathering_EastAtrium(1113,1040);
        const QPoint Totem_GalleryHall(424,301);
        const QPoint WateryHall_Gathering(1093,825);
        const QPoint EastAtrium_CoreAccess(1167,1116);
        const QPoint WateryHall_DynamoHall(1129,864);
        const QPoint EnergyCore_Access(1249,1152);
        const QPoint EnergyCore_FurnaceHall(1354,1171);
        const QPoint EnergyCore_BurningHall(1349,1039);
        const QPoint Dynamo_WateryHall(1169,949);
        const QPoint BurnDome_EnergyHall(1356,915);
        const QPoint Furnace_CoreHall(1432,1214);
        const QPoint Furnace_ElderHall(1612,1222);
        const QPoint Furnace_CrosswayHall(1561,1114);
        const QPoint Elders_FurnaceHall(1689,1180);
        const QPoint Crossway_FurnaceHall(1667,999);
        const QPoint Crossway_EldersWhite(1720,1031);
        const QPoint Crossway_EldersMissile(1803,1016);
        const QPoint Elders_ReflectingHall(1745,1250);
        const QPoint Elders_CrosswayMissile(1756,1128);
        const QPoint Elders_CrosswayWhite(1720,1123);
        const QPoint Elders_Chamber(1720,1227);
        const QPoint ReflectPool_EldersHall(1706,1334);
        const QPoint ReflectPool_SaveStation(1672,1425);
        const QPoint ReflectPool_TransportHall(1705,1497);
        const QPoint ReflectPool_Antechamber(1738,1425);
        const QPoint Tallon_SaveStation(1532,1399);
        const QPoint Tallon_ExitHall(1701,1577);
        const QPoint Sunchamber_SunTower(848,450);

        points["Transport to Tallon Overworld North"] = {
            DoorPoint(Tallon_Entrance,Down)
        };
        points["Ruins Entrance"] = {
            DoorPoint(Plaza_Entrance,Down),
            DoorPoint(Tallon_Entrance,Up)
        };
        points["Main Plaza"] = {
            DoorPoint(Plaza_FountainHall,Up),
            DoorPoint(Plaza_Entrance,Up),
            DoorPoint(Plaza_ShrineHall,Right),
            DoorPoint(Plaza_NurseryHall,Right),
            DoorPoint(Plaza_VaultHall,Left),
            DoorPoint(Plaza_PistonHall,Up)
        };
        points["Ruined Fountain Access"] = {
            DoorPoint(Plaza_FountainHall,Down),
            DoorPoint(Fountain_PlazaHall,Up)
        };
        points["Ruined Shrine Access"] = {
            DoorPoint(Shrine_PlazaHall,Right),
            DoorPoint(Plaza_ShrineHall,Left)
        };
        points["Nursery Access"] = {
            DoorPoint(Eyon_PlazaHall,Right),
            DoorPoint(Plaza_NurseryHall,Left)
        };
        points["Plaza Access"] = {
            DoorPoint(Plaza_VaultHall,Right),
            DoorPoint(Vault_PlazaHall,Down)
        };
        points["Ruined Fountain"] = {
            DoorPoint(Fountain_Arboretum,Left),
            DoorPoint(Fountain_PlazaHall,Down),
            DoorPoint(Fountain_Meditation,Up)
        };
        points["Ruined Shrine"] = {
            DoorPoint(Shrine_TowerHall,Right),
            DoorPoint(Shrine_PlazaHall,Left)
        };
        points["Eyon Tunnel"] = {
            DoorPoint(Eyon_Nursery,Right),
            DoorPoint(Eyon_PlazaHall,Left)
        };
        points["Vault"] = {
            DoorPoint(Vault_ElevatorHall,Down),
            DoorPoint(Vault_PlazaHall,Up)
        };
        points["Training Chamber"] = {
            DoorPoint(Training_MagmaHall,Up),
            DoorPoint(Training_PistonHall,Down)
        };
        points["Arboretum Access"] = {
            DoorPoint(Arboretum_Fountain,Left),
            DoorPoint(Fountain_Arboretum,Right)
        };
        points["Meditation Fountain"] = {
            DoorPoint(Magma_MeditationHall,Up),
            DoorPoint(Fountain_Meditation,Down)
        };
        points["Tower of Light Access"] = {
            DoorPoint(LightTower_ShrineHall,Up),
            DoorPoint(Shrine_TowerHall,Left)
        };
        points["Ruined Nursery"] = {
            DoorPoint(Nursery_SaveStation,Up),
            DoorPoint(Nursery_NorthAtrium,Down),
            DoorPoint(Eyon_Nursery,Left)
        };
        points["Vault Access"] = {
            DoorPoint(Vault_ElevatorHall,Up),
            DoorPoint(Magmoor_VaultHall,Down)
        };
        points["Training Chamber Access"] = {
            DoorPoint(Magma_TrainingHall,Left),
            DoorPoint(Training_MagmaHall,Down)
        };
        points["Arboretum"] = {
            DoorPoint(Arboretum_SunLobby,Down),
            DoorPoint(Arboretum_Fountain,Right),
            DoorPoint(Arboretum_Gathering,Left)
        };
        points["Magma Pool"] = {
            DoorPoint(Magma_TrainingHall,Right),
            DoorPoint(Magma_MeditationHall,Down)
        };
        points["Tower of Light"] = {
            DoorPoint(LightTower_ShrineHall,Down),
            DoorPoint(LightTower_Chamber,Bottom)
        };
        points["Save Station 1"] = {
            DoorPoint(Nursery_SaveStation,Down)
        };
        points["North Atrium"] = {
            DoorPoint(Nursery_NorthAtrium,Up),
            DoorPoint(Gallery_NorthAtrium,Down)
        };
        points["Transport to Magmoor Caverns North"] = {
            DoorPoint(Magmoor_SunTower,Left),
            DoorPoint(Magmoor_VaultHall,Up),
            DoorPoint(Magmoor_TotemHall,Right)
        };
        points["Sunchamber Lobby"] = {
            DoorPoint(Arboretum_SunLobby,Up),
            DoorPoint(SunLobby_SunAccess,Down)
        };
        points["Gathering Hall Access"] = {
            DoorPoint(Gathering_ArboretumHall,Left),
            DoorPoint(Arboretum_Gathering,Right)
        };
        points["Tower Chamber"] = {
            DoorPoint(LightTower_Chamber,Top)
        };
        points["Ruined Gallery"] = {
            DoorPoint(Gallery_NorthAtrium,Up),
            DoorPoint(Gallery_TotemHall,Down),
            DoorPoint(Gallery_Map,Right)
        };
        points["Sun Tower"] = {
            DoorPoint(SunTower_SunChamber,Left),
            DoorPoint(Magmoor_SunTower,Right)
        };
        points["Transport Access North"] = {
            DoorPoint(Totem_MagmoorHall,Up),
            DoorPoint(Magmoor_TotemHall,Left)
        };
        points["Sunchamber Access"] = {
            DoorPoint(Sunchamber_Access,Down),
            DoorPoint(SunLobby_SunAccess,Up)
        };
        points["Gathering Hall"] = {
            DoorPoint(Gathering_WateryHall,Down),
            DoorPoint(Gathering_ArboretumHall,Right),
            DoorPoint(Gathering_SaveStation,Up),
            DoorPoint(Gathering_EastAtrium,Left)
        };
        points["Totem Access"] = {
            DoorPoint(Gallery_TotemHall,Up),
            DoorPoint(Totem_GalleryHall,Left)
        };
        points["Map Station"] = {
            DoorPoint(Gallery_Map,Left)
        };
        points["Sun Tower Access"] = {
            DoorPoint(Sunchamber_SunTower,Up),
            DoorPoint(SunTower_SunChamber,Right)
        };
        points["Hive Totem"] = {
            DoorPoint(Totem_GalleryHall,Right),
            DoorPoint(Totem_MagmoorHall,Down)
        };
        points["Sunchamber"] = {
            DoorPoint(Sunchamber_SunTower,Down),
            DoorPoint(Sunchamber_Access,Up)
        };
        points["Watery Hall Access"] = {
            DoorPoint(Gathering_WateryHall,Up),
            DoorPoint(WateryHall_Gathering,Down)
        };
        points["Save Station 2"] = {
            DoorPoint(Gathering_SaveStation,Down)
        };
        points["East Atrium"] = {
            DoorPoint(Gathering_EastAtrium,Right),
            DoorPoint(EastAtrium_CoreAccess,Left)
        };
        points["Watery Hall"] = {
            DoorPoint(WateryHall_DynamoHall,Up),
            DoorPoint(WateryHall_Gathering,Up)
        };
        points["Energy Core Access"] = {
            DoorPoint(WateryHall_DynamoHall,Down),
            DoorPoint(Dynamo_WateryHall,Up)
        };
        points["Dynamo Access"] = {
            DoorPoint(EnergyCore_Access,Left),
            DoorPoint(EastAtrium_CoreAccess,Right)
        };
        points["Energy Core"] = {
            DoorPoint(EnergyCore_BurningHall,Down),
            DoorPoint(EnergyCore_FurnaceHall,Left),
            DoorPoint(EnergyCore_Access,Right)
        };
        points["Dynamo"] = {
            DoorPoint(Dynamo_WateryHall,Down)
        };
        points["Burn Dome Access"] = {
            DoorPoint(BurnDome_EnergyHall,Down),
            DoorPoint(EnergyCore_BurningHall,Up)
        };
        points["West Furnace Access"] = {
            DoorPoint(EnergyCore_FurnaceHall,Right),
            DoorPoint(Furnace_CoreHall,Left)
        };
        points["Burn Dome"] = {
            DoorPoint(BurnDome_EnergyHall,Up)
        };
        points["Furnace"] = {
            DoorPoint(Furnace_ElderHall,Left),
            DoorPoint(Furnace_CoreHall,Right),
            DoorPoint(Furnace_CrosswayHall,Down)
        };
        points["East Furnace Access"] = {
            DoorPoint(Elders_FurnaceHall,Left),
            DoorPoint(Furnace_ElderHall,Right)
        };
        points["Crossway Access West"] = {
            DoorPoint(Crossway_FurnaceHall,Left),
            DoorPoint(Furnace_CrosswayHall,Up)
        };
        points["Hall of the Elders"] = {
            DoorPoint(Elders_ReflectingHall,Up),
            DoorPoint(Elders_CrosswayMissile,Down),
            DoorPoint(Elders_FurnaceHall,Right),
            DoorPoint(Elders_CrosswayWhite,Down),
            DoorPoint(Elders_Chamber,Top)
        };
        points["Crossway"] = {
            DoorPoint(Crossway_EldersWhite,Up),
            DoorPoint(Crossway_FurnaceHall,Right),
            DoorPoint(Crossway_EldersMissile,Up)
        };
        points["Reflecting Pool Access"] = {
            DoorPoint(ReflectPool_EldersHall,Up),
            DoorPoint(Elders_ReflectingHall,Down)
        };
        points["Elder Hall Access"] = {
            DoorPoint(Elders_CrosswayMissile,Up),
            DoorPoint(Crossway_EldersMissile,Down)
        };
        points["Crossway Access South"] = {
            DoorPoint(Elders_CrosswayWhite,Up),
            DoorPoint(Crossway_EldersWhite,Down)
        };
        points["Elder Chamber"] = {
            DoorPoint(Elders_Chamber,Bottom)
        };
        points["Reflecting Pool"] = {
            DoorPoint(ReflectPool_SaveStation,Right),
            DoorPoint(ReflectPool_TransportHall,Up),
            DoorPoint(ReflectPool_EldersHall,Down),
            DoorPoint(ReflectPool_Antechamber,Left)
        };
        points["Save Station 3"] = {
            DoorPoint(ReflectPool_SaveStation,Left),
            DoorPoint(Tallon_SaveStation,Right)
        };
        points["Transport Access South"] = {
            DoorPoint(ReflectPool_TransportHall,Down),
            DoorPoint(Tallon_ExitHall,Up)
        };
        points["Antechamber"] = {
            DoorPoint(ReflectPool_Antechamber,Right)
        };
        points["Transport to Tallon Overworld East"] = {
            DoorPoint(Tallon_SaveStation,Left)
        };
        points["Transport to Tallon Overworld South"] = {
            DoorPoint(Tallon_ExitHall,Down)
        };
        points["Piston Hall"] = {
            DoorPoint(Plaza_PistonHall,Down),
            DoorPoint(Training_PistonHall,Up)
        };
    } else if (world == MagmoorCaverns) {

        const QPoint BurnTrail_Chozo(171,253);
        const QPoint BurnTrail_LakeHall(239,611);
        const QPoint BurnTrail_SaveStation(227,432);
        const QPoint LavaLake_BurnHall(287,758);
        const QPoint LavaLake_PitHall(564,906);
        const QPoint Triclops_LakeHall(502,1029);
        const QPoint Triclops_MonitorHall(711,1130);
        const QPoint Triclops_Storage(392,1328);
        const QPoint Monitor_TriclopsHall(820,1249);
        const QPoint Monitor_PhendranaHall(703,1405);
        const QPoint Monitor_WarriorShrineA(913,1436); // Separate pair
        const QPoint Monitor_WarriorShrineB(948,1279); // Separate pair
        const QPoint Monitor_ShoreTunnel(905,1388);
        const QPoint Phendrana_MonitorHall(569,1406);
        const QPoint Warrior_Shores(1016,1277);
        const QPoint Shores_Warrior(976,1436);
        const QPoint Shore_MonitorTunnel(988,1520);
        const QPoint Shores_TallonHall(1075,1749);
        const QPoint Tallon_ShoresHall(1075,1892);
        const QPoint Twin_TallonTunnel(983,2144);
        const QPoint Tallon_TwinTunnel(1074,1995);
        const QPoint Twin_CoreTunnel(905,2438);
        const QPoint GeoCore_TwinTunnel(1015,2558);
        const QPoint Plasma_Processing(1091,2476);
        const QPoint GeoCore_WorkTunnel(1238,2584);
        const QPoint Workstation_CoreTunnel(1286,2731);
        const QPoint Workstation_Phazon(1222,2955);
        const QPoint Workstation_Phendrana(1132,2842);
        const QPoint Phazon_Workstation(1298,3120);
        const QPoint Phendrana_Workstation(1050,2798);
        const QPoint Phendrana_SaveStation(973,2798);

        points["Transport to Chozo Ruins North"] = {
            DoorPoint(BurnTrail_Chozo,Up)
        };
        points["Burning Trail"] = {
            DoorPoint(BurnTrail_LakeHall,Up),
            DoorPoint(BurnTrail_Chozo,Down),
            DoorPoint(BurnTrail_SaveStation,Down)
        };
        points["Lake Tunnel"] = {
            DoorPoint(LavaLake_BurnHall,Left),
            DoorPoint(BurnTrail_LakeHall,Down)
        };
        points["Save Station Magmoor A"] = {
            DoorPoint(BurnTrail_SaveStation,Up)
        };
        points["Lava Lake"] = {
            DoorPoint(LavaLake_BurnHall,Right),
            DoorPoint(LavaLake_PitHall,Up)
        };
        points["Pit Tunnel"] = {
            DoorPoint(Triclops_LakeHall,Up),
            DoorPoint(LavaLake_PitHall,Down)
        };
        points["Triclops Pit"] = {
            DoorPoint(Triclops_MonitorHall,Left),
            DoorPoint(Triclops_Storage,Up),
            DoorPoint(Triclops_LakeHall,Down)
        };
        points["Monitor Tunnel"] = {
            DoorPoint(Monitor_TriclopsHall,Up),
            DoorPoint(Triclops_MonitorHall,Right)
        };
        points["Storage Cavern"] = {
            DoorPoint(Triclops_Storage,Down)
        };
        points["Monitor Station"] = {
            DoorPoint(Monitor_TriclopsHall,Down),
            DoorPoint(Monitor_PhendranaHall,Right),
            DoorPoint(Monitor_WarriorShrineA,Left),
            DoorPoint(Monitor_ShoreTunnel,Left)
        };
        points["Transport Tunnel A"] = {
            DoorPoint(Phendrana_MonitorHall,Right),
            DoorPoint(Monitor_PhendranaHall,Left)
        };
        points["Warrior Shrine"] = {
            DoorPoint(Monitor_WarriorShrineB,Right),
            DoorPoint(Warrior_Shores,Center)
        };
        points["Shore Tunnel"] = {
            DoorPoint(Monitor_ShoreTunnel,Right),
            DoorPoint(Shore_MonitorTunnel,Left)
        };
        points["Transport to Phendrana Drifts North"] = {
            DoorPoint(Phendrana_MonitorHall,Left)
        };
        points["Fiery Shores"] = {
            DoorPoint(Shore_MonitorTunnel,Right),
            DoorPoint(Shores_TallonHall,Up),
            DoorPoint(Shores_Warrior,Center)
        };
        points["Transport Tunnel B"] = {
            DoorPoint(Tallon_ShoresHall,Up),
            DoorPoint(Shores_TallonHall,Down)
        };
        points["Transport to Tallon Overworld West"] = {
            DoorPoint(Tallon_TwinTunnel,Up),
            DoorPoint(Tallon_ShoresHall,Down)
        };
        points["Twin Fires Tunnel"] = {
            DoorPoint(Twin_TallonTunnel,Up),
            DoorPoint(Tallon_TwinTunnel,Down)
        };
        points["Twin Fires"] = {
            DoorPoint(Twin_CoreTunnel,Up),
            DoorPoint(Twin_TallonTunnel,Down)
        };
        points["North Core Tunnel"] = {
            DoorPoint(Twin_CoreTunnel,Down),
            DoorPoint(GeoCore_TwinTunnel,Left)
        };
        points["Geothermal Core"] = {
            DoorPoint(GeoCore_TwinTunnel,Right),
            DoorPoint(Plasma_Processing,Down),
            DoorPoint(GeoCore_WorkTunnel,Left)
        };
        points["Plasma Processing"] = {
            DoorPoint(Plasma_Processing,Up)
        };
        points["South Core Tunnel"] = {
            DoorPoint(GeoCore_WorkTunnel,Right),
            DoorPoint(Workstation_CoreTunnel,Up)
        };
        points["Magmoor Workstation"] = {
            DoorPoint(Workstation_CoreTunnel,Down),
            DoorPoint(Workstation_Phazon,Up),
            DoorPoint(Workstation_Phendrana,Right)
        };
        points["Workstation Tunnel"] = {
            DoorPoint(Phazon_Workstation,Left),
            DoorPoint(Workstation_Phazon,Down)
        };
        points["Transport Tunnel C"] = {
            DoorPoint(Phendrana_Workstation,Right),
            DoorPoint(Workstation_Phendrana,Left)
        };
        points["Transport to Phazon Mines West"] = {
            DoorPoint(Phazon_Workstation,Right)
        };
        points["Transport to Phendrana Drifts South"] = {
            DoorPoint(Phendrana_SaveStation,Right),
            DoorPoint(Phendrana_Workstation,Left)
        };
        points["Save Station Magmoor B"] = {
            DoorPoint(Phendrana_SaveStation,Left)
        };
    } else if (world == PhendranaDrifts) {

        const QPoint Magmoor_Shorelines(827,197);
        const QPoint Shorelines_Magmoor(827,419);
        const QPoint Shorelines_IceTemple(685,504);
        const QPoint Shorelines_SaveStation(804,709);
        const QPoint Shorelines_RuinsWest(806,683);
        const QPoint Shorelines_Walkway(886,669);
        const QPoint Shorelines_RuinsEast(909,579);
        const QPoint IceTemple_Shorelines(520,456);
        const QPoint RuinsWest_Shorelines(825,818);
        const QPoint Walkway_RuinsEast(1074,756);
        const QPoint RuinsEast_Shorelines(1074,628);
        const QPoint IceTemple_Chapel(376,604);
        const QPoint RuinsWest_Courtyard(879,997);
        const QPoint RuinsWest_Canyon(743,980);
        const QPoint Chapel_IceTemple(226,605);
        const QPoint Canyon_RuinsWest(578,932);
        const QPoint Courtyard_RuinsWest(846,1188);
        const QPoint Courtyard_SaveStation(778,1242);
        const QPoint Courtyard_Research(896,1346);
        const QPoint Courtyard_Quarantine(998,1244);
        const QPoint Research_Courtyard(898,1488);
        const QPoint North_Quarantine(1071,1360);
        const QPoint Research_MapStation(835,1660);
        const QPoint Research_Hydra(898,1703);
        const QPoint Quarantine_North(1130,1613);
        const QPoint Quarantine_South(1370,1783);
        const QPoint Quarantine_Monitor(1490,1558);
        const QPoint Hydra_Research(854,1881);
        const QPoint Hydra_Observatory(799,1930);
        const QPoint Quarantine_Magmoor(1266,2050);
        const QPoint Observatory_Hydra(760,2068);
        const QPoint Magmoor_Pike(1265,2162);
        const QPoint Observatory_Tower1(759,2090); // Separate pair
        const QPoint Observatory_Save(760,2298);
        const QPoint Pike_Magmoor(1265,2318);
        const QPoint Observatory_Tower2(377,1947); // Separate pair
        const QPoint Tower_Observatory(422,1853);
        const QPoint Pike_Research(1201,2406);
        const QPoint Pike_Frost(1265-10,2470);
        const QPoint Pike_Hunter(1265+10,2470);
        const QPoint Tower_West(460,1801);
        const QPoint Tower_East(558,1801);
        const QPoint Research_Pike(1076,2412);
        const QPoint Frost_Pike(1206,2836);
        const QPoint Hunter_Pike(1332,2836);
        const QPoint Core_Aether(1007,2336);
        const QPoint Frost_SaveStation(1077,2929);
        const QPoint Frost_Edge(1173,3046);
        const QPoint Hunter_ChamberHall(1442,2883);
        const QPoint Hunter_LakeHall(1452,2924);
        const QPoint Hunter_Edge(1319,3011);
        const QPoint Aether_Core(982,2248);
        const QPoint Edge_Frost(1232-10,3223);
        const QPoint Edge_Hunter(1232+10,3223);
        const QPoint Gravity_OverWater(1566,2834);
        const QPoint Gravity_UnderWater(1583,2924);
        const QPoint Tower_Aether(595,1852);
        const QPoint Aether_Tower1(639,1947);  // Separate pair
        const QPoint Aether_Tower2(1020,2090); // Separate pair
        const QPoint Edge_Storage(1300,3297);
        const QPoint Edge_Security(1231,3369);

        points["Transport to Magmoor Caverns West"] = {
            DoorPoint(Magmoor_Shorelines,Up)
        };
        points["Shoreline Entrance"] = {
            DoorPoint(Magmoor_Shorelines,Down),
            DoorPoint(Shorelines_Magmoor,Up)
        };
        points["Phendrana Shorelines"] = {
            DoorPoint(Shorelines_Magmoor,Down),
            DoorPoint(Shorelines_IceTemple,Right),
            DoorPoint(Shorelines_SaveStation,Right),
            DoorPoint(Shorelines_RuinsWest,Up),
            DoorPoint(Shorelines_Walkway,Up),
            DoorPoint(Shorelines_RuinsEast,Left)
        };
        points["Temple Entryway"] = {
            DoorPoint(IceTemple_Shorelines,Right),
            DoorPoint(Shorelines_IceTemple,Left)
        };
        points["Save Station B"] = {
            DoorPoint(Shorelines_SaveStation,Left)
        };
        points["Ruins Entryway"] = {
            DoorPoint(RuinsWest_Shorelines,Up),
            DoorPoint(Shorelines_RuinsWest,Down)
        };
        points["Plaza Walkway"] = {
            DoorPoint(Shorelines_Walkway,Down),
            DoorPoint(Walkway_RuinsEast,Left)
        };
        points["Ice Ruins Access"] = {
            DoorPoint(RuinsEast_Shorelines,Left),
            DoorPoint(Shorelines_RuinsEast,Right)
        };
        points["Chozo Ice Temple"] = {
            DoorPoint(IceTemple_Shorelines,Left),
            DoorPoint(IceTemple_Chapel,Right)
        };
        points["Ice Ruins West"] = {
            DoorPoint(RuinsWest_Shorelines,Down),
            DoorPoint(RuinsWest_Courtyard,Up),
            DoorPoint(RuinsWest_Canyon,Right)
        };
        points["Ice Ruins East"] = {
            DoorPoint(RuinsEast_Shorelines,Right),
            DoorPoint(Walkway_RuinsEast,Right)
        };
        points["Chapel Tunnel"] = {
            DoorPoint(Chapel_IceTemple,Right),
            DoorPoint(IceTemple_Chapel,Left)
        };
        points["Courtyard Entryway"] = {
            DoorPoint(Courtyard_RuinsWest,Up),
            DoorPoint(RuinsWest_Courtyard,Down)
        };
        points["Canyon Entryway"] = {
            DoorPoint(Canyon_RuinsWest,Right),
            DoorPoint(RuinsWest_Canyon,Left)
        };
        points["Chapel of the Elders"] = {
            DoorPoint(Chapel_IceTemple,Left)
        };
        points["Ruined Courtyard"] = {
            DoorPoint(Courtyard_RuinsWest,Down),
            DoorPoint(Courtyard_SaveStation,Right),
            DoorPoint(Courtyard_Research,Up),
            DoorPoint(Courtyard_Quarantine,Left)
        };
        points["Phendrana Canyon"] = {
            DoorPoint(Canyon_RuinsWest,Left)
        };
        points["Save Station A"] = {
            DoorPoint(Courtyard_SaveStation,Left)
        };
        points["Specimen Storage"] = {
            DoorPoint(Research_Courtyard,Up),
            DoorPoint(Courtyard_Research,Down)
        };
        points["Quarantine Access"] = {
            DoorPoint(North_Quarantine,Up),
            DoorPoint(Courtyard_Quarantine,Right)
        };
        points["Research Entrance"] = {
            DoorPoint(Research_Courtyard,Down),
            DoorPoint(Research_MapStation,Right),
            DoorPoint(Research_Hydra,Up)
        };
        points["North Quarantine Tunnel"] = {
            DoorPoint(Quarantine_North,Left),
            DoorPoint(North_Quarantine,Down)
        };
        points["Map Station"] = {
            DoorPoint(Research_MapStation,Left)
        };
        points["Hydra Lab Entryway"] = {
            DoorPoint(Research_Hydra,Down),
            DoorPoint(Hydra_Research,Up)
        };
        points["Quarantine Cave"] = {
            DoorPoint(Quarantine_North,Right),
            DoorPoint(Quarantine_South,Up),
            DoorPoint(Quarantine_Monitor,Left)
        };
        points["Research Lab Hydra"] = {
            DoorPoint(Hydra_Research,Down),
            DoorPoint(Hydra_Observatory,Bottom)
        };
        points["South Quarantine Tunnel"] = {
            DoorPoint(Quarantine_South,Down),
            DoorPoint(Quarantine_Magmoor,Up)
        };
        points["Quarantine Monitor"] = {
            DoorPoint(Quarantine_Monitor,Right)
        };
        points["Observatory Access"] = {
            DoorPoint(Observatory_Hydra,Up),
            DoorPoint(Hydra_Observatory,Top)
        };
        points["Transport to Magmoor Caverns South"] = {
            DoorPoint(Magmoor_Pike,Up),
            DoorPoint(Quarantine_Magmoor,Down)
        };
        points["Observatory"] = {
            DoorPoint(Observatory_Hydra,Down),
            DoorPoint(Observatory_Tower1,Down),
            DoorPoint(Observatory_Save,Up)
        };
        points["Transport Access"] = {
            DoorPoint(Magmoor_Pike,Down),
            DoorPoint(Pike_Magmoor,Up)
        };
        points["West Tower Entrance"] = {
            DoorPoint(Observatory_Tower2,Up),
            DoorPoint(Tower_Observatory,Down)
        };
        points["Save Station D"] = {
            DoorPoint(Observatory_Save,Down)
        };
        points["Frozen Pike"] = {
            DoorPoint(Pike_Research,Right),
            DoorPoint(Pike_Magmoor,Down),
            DoorPoint(Pike_Frost,Up),
            DoorPoint(Pike_Hunter,Up)
        };
        points["West Tower "] = {
            DoorPoint(Tower_Observatory,Up),
            DoorPoint(Tower_West,Left)
        };
        points["Pike Access"] = {
            DoorPoint(Pike_Research,Left),
            DoorPoint(Research_Pike,Right)
        };
        points["Frost Cave Access"] = {
            DoorPoint(Pike_Frost,Down),
            DoorPoint(Frost_Pike,Up)
        };
        points["Hunter Cave Access"] = {
            DoorPoint(Pike_Hunter,Down),
            DoorPoint(Hunter_Pike,Up)
        };
        points["Control Tower"] = {
            DoorPoint(Tower_East,Left),
            DoorPoint(Tower_West,Right)
        };
        points["Research Core"] = {
            DoorPoint(Research_Pike,Left),
            DoorPoint(Core_Aether,Down)
        };
        points["Frost Cave"] = {
            DoorPoint(Frost_SaveStation,Right),
            DoorPoint(Frost_Pike,Down),
            DoorPoint(Frost_Edge,Up)
        };
        points["Hunter Cave"] = {
            DoorPoint(Hunter_Pike,Down),
            DoorPoint(Hunter_Edge,Up),
            DoorPoint(Hunter_ChamberHall,Left),
            DoorPoint(Hunter_LakeHall,Left)
        };
        points["East Tower"] = {
            DoorPoint(Tower_Aether,Up),
            DoorPoint(Tower_East,Right),
        };
        points["Research Core Access"] = {
            DoorPoint(Core_Aether,Up),
            DoorPoint(Aether_Core,Bottom)
        };
        points["Save Station C"] = {
            DoorPoint(Frost_SaveStation,Left)
        };
        points["Upper Edge Tunnel"] = {
            DoorPoint(Edge_Frost,Up),
            DoorPoint(Frost_Edge,Down)
        };
        points["Lower Edge Tunnel"] = {
            DoorPoint(Edge_Hunter,Up),
            DoorPoint(Hunter_Edge,Down)
        };
        points["Chamber Access"] = {
            DoorPoint(Hunter_ChamberHall,Right),
            DoorPoint(Gravity_OverWater,Left)
        };
        points["Lake Tunnel"] = {
            DoorPoint(Hunter_LakeHall,Right),
            DoorPoint(Gravity_UnderWater,Left)
        };
        points["Aether Lab Entryway"] = {
            DoorPoint(Tower_Aether,Down),
            DoorPoint(Aether_Tower1,Up)
        };
        points["Research Lab Aether"] = {
            DoorPoint(Aether_Tower2,Down),
            DoorPoint(Aether_Core,Top)
        };
        points["Phendrana's Edge"] = {
            DoorPoint(Edge_Hunter,Down),
            DoorPoint(Edge_Frost,Down),
            DoorPoint(Edge_Storage,Left),
            DoorPoint(Edge_Security,Up)
        };
        points["Gravity Chamber"] = {
            DoorPoint(Gravity_UnderWater,Right),
            DoorPoint(Gravity_OverWater,Right)
        };
        points["Storage Cave"] = {
            DoorPoint(Edge_Storage,Right)
        };
        points["Security Cave"] = {
            DoorPoint(Edge_Security,Down)
        };
    } else if (world == PhazonMines) {

        const QPoint Tallon_Quarry(1629,425);
        const QPoint Quarry_Tallon(1516,466);
        const QPoint Quarry_Ore(1348,427);
        const QPoint Quarry_SaveStation(1395,618);
        const QPoint Quarry_Security(1294,620);
        const QPoint Ore_Quarry(1329,358);
        const QPoint Security_Quarry(1188,651);
        const QPoint Ore_ResearchHall(1264,289+10);
        const QPoint Ore_GrappleBeam(1329,220);
        const QPoint Ore_Elevator(1264,289-10);
        const QPoint Security_Research(1128,514);
        const QPoint Security_Depot(1128,498);
        const QPoint Research_OreHall(1179,291);
        const QPoint ElevatorA_Ore(1214,222);
        const QPoint Research_Security(1089,365);
        const QPoint ElevatorA_EliteControl(1230,1309);
        const QPoint EliteControl_ElevatorA(1311,1399);
        const QPoint EliteControl_Maintenance(1159,1471);
        const QPoint EliteControl_Omega(1202,1528);
        const QPoint Maintenance_EliteControl(1026,1539);
        const QPoint Omega_EliteControl(1249,1653);
        const QPoint Processing_Maintenance(334,1119);
        const QPoint Processing_Magmoor(231,1033);
        const QPoint Processing_Quarters(238,1057);
        const QPoint Omega_MapStation(1210,1823);
        const QPoint Omega_DynamoHall(1283,1802);
        const QPoint Magmoor_Processing(230,908);
        const QPoint Quarters_Processing(176,1156);
        const QPoint Dynamo_OmegaHall(1398,1750);
        const QPoint Quarters_MetroidHall(177,1419);
        const QPoint Dynamo_QuarantineHall(1464,1546);
        const QPoint Dynamo_SaveStation(1469,1759);
        const QPoint Metroid_QuartersHall(265,1484);
        const QPoint Quarantine_DynamoHall(1417,1421);
        const QPoint Metroid_FungalHall(597,1453);
        const QPoint Metroid_SaveStation(324,1620);
        const QPoint Quarantine_ElevatorB(1500,1061);
        const QPoint ElevatorB_Quarantine(1462,942);
        const QPoint FungalHall_Metroid(675,1389);
        const QPoint FungalHall_Missile(813,1290);
        const QPoint FungalHall_Phazon(751,1032);
        const QPoint ElevatorB_FungalHall(770,521);
        const QPoint FungalHall_ElevatorB(739,630);
        const QPoint Phazon_FungalHall(662,973);

        points["Transport to Tallon Overworld South"] = {
            DoorPoint(Tallon_Quarry,Right)
        };
        points["Quarry Access"] = {
            DoorPoint(Quarry_Tallon,Right),
            DoorPoint(Tallon_Quarry,Left)
        };
        points["Main Quarry"] = {
            DoorPoint(Quarry_Ore,Down),
            DoorPoint(Quarry_Tallon,Left),
            DoorPoint(Quarry_SaveStation,Up),
            DoorPoint(Quarry_Security,Up)
        };
        points["Waste Disposal"] = {
            DoorPoint(Quarry_Ore,Up),
            DoorPoint(Ore_Quarry,Down)
        };
        points["Save Station Mines A"] = {
            DoorPoint(Quarry_SaveStation,Down)
        };
        points["Security Access A"] = {
            DoorPoint(Quarry_Security,Down),
            DoorPoint(Security_Quarry,Right)
        };
        points["Ore Processing"] = {
            DoorPoint(Ore_ResearchHall,Right),
            DoorPoint(Ore_GrappleBeam,Down),
            DoorPoint(Ore_Quarry,Up),
            DoorPoint(Ore_Elevator,Right)
        };
        points["Mine Security Station"] = {
            DoorPoint(Security_Quarry,Left),
            DoorPoint(Security_Research,Bottom),
            DoorPoint(Security_Depot,Down)
        };
        points["Research Access"] = {
            DoorPoint(Ore_ResearchHall,Left),
            DoorPoint(Research_OreHall,Right)
        };
        points["Storage Depot B"] = {
            DoorPoint(Ore_GrappleBeam,Up)
        };
        points["Elevator Access A"] = {
            DoorPoint(Ore_Elevator,Left),
            DoorPoint(ElevatorA_Ore,Right)
        };
        points["Storage Depot A"] = {
            DoorPoint(Security_Depot,Up)
        };
        points["Elite Research"] = {
            DoorPoint(Research_OreHall,Left),
            DoorPoint(Research_Security,Top)
        };
        points["Security Access B"] = {
            DoorPoint(Research_Security,Bottom),
            DoorPoint(Security_Research,Top)
        };
        points["Elevator A"] = {
            DoorPoint(ElevatorA_Ore,Left),
            DoorPoint(ElevatorA_EliteControl,Left)
        };
        points["Elite Control Access"] = {
            DoorPoint(ElevatorA_EliteControl,Right),
            DoorPoint(EliteControl_ElevatorA,Up)
        };
        points["Elite Control"] = {
            DoorPoint(EliteControl_Maintenance,Right),
            DoorPoint(EliteControl_ElevatorA,Down),
            DoorPoint(EliteControl_Omega,Bottom)
        };
        points["Maintenance Tunnel"] = {
            DoorPoint(EliteControl_Maintenance,Left),
            DoorPoint(Maintenance_EliteControl,Down)
        };
        points["Ventilation Shaft"] = {
            DoorPoint(Omega_EliteControl,Up),
            DoorPoint(EliteControl_Omega,Top)
        };
        points["Phazon Processing Center"] = {
            DoorPoint(Processing_Magmoor,Down),
            DoorPoint(Processing_Maintenance,Up),
            DoorPoint(Processing_Quarters,Right)
        };
        points["Omega Research"] = {
            DoorPoint(Omega_MapStation,Up),
            DoorPoint(Omega_EliteControl,Down),
            DoorPoint(Omega_DynamoHall,Top)
        };
        points["Transport Access"] = {
            DoorPoint(Processing_Magmoor,Up),
            DoorPoint(Magmoor_Processing,Down)
        };
        points["Processing Center Access"] = {
            DoorPoint(Processing_Quarters,Left),
            DoorPoint(Quarters_Processing,Up)
        };
        points["Map Station Mines"] = {
            DoorPoint(Omega_MapStation,Down)
        };
        points["Dynamo Access"] = {
            DoorPoint(Dynamo_OmegaHall,Left),
            DoorPoint(Omega_DynamoHall,Bottom)
        };
        points["Transport to Magmoor Caverns South"] = {
            DoorPoint(Magmoor_Processing,Up)
        };
        points["Elite Quarters"] = {
            DoorPoint(Quarters_MetroidHall,Up),
            DoorPoint(Quarters_Processing,Down)
        };
        points["Central Dynamo"] = {
            DoorPoint(Dynamo_OmegaHall,Right),
            DoorPoint(Dynamo_QuarantineHall,Down),
            DoorPoint(Dynamo_SaveStation,Up)
        };
        points["Elite Quarters Access"] = {
            DoorPoint(Metroid_QuartersHall,Left),
            DoorPoint(Quarters_MetroidHall,Down)
        };
        points["Quarantine Access A"] = {
            DoorPoint(Dynamo_QuarantineHall,Up),
            DoorPoint(Quarantine_DynamoHall,Down)
        };
        points["Save Station Mines B"] = {
            DoorPoint(Dynamo_SaveStation,Down)
        };
        points["Metroid Quarantine B"] = {
            DoorPoint(Metroid_FungalHall,Left),
            DoorPoint(Metroid_QuartersHall,Right),
            DoorPoint(Metroid_SaveStation,Up)
        };
        points["Metroid Quarantine A"] = {
            DoorPoint(Quarantine_DynamoHall,Up),
            DoorPoint(Quarantine_ElevatorB,Down)
        };
        points["Quarantine Access B"] = {
            DoorPoint(Metroid_FungalHall,Right),
            DoorPoint(FungalHall_Metroid,Down)
        };
        points["Save Station Mines C"] = {
            DoorPoint(Metroid_SaveStation,Down)
        };
        points["Elevator Access B"] = {
            DoorPoint(Quarantine_ElevatorB,Up),
            DoorPoint(ElevatorB_Quarantine,Down)
        };
        points["Fungal Hall B"] = {
            DoorPoint(FungalHall_Missile,Left),
            DoorPoint(FungalHall_Metroid,Up),
            DoorPoint(FungalHall_Phazon,Down)
        };
        points["Elevator B"] = {
            DoorPoint(ElevatorB_Quarantine,Up),
            DoorPoint(ElevatorB_FungalHall,Up)
        };
        points["Missile Station Mines"] = {
            DoorPoint(FungalHall_Missile,Right)
        };
        points["Phazon Mining Tunnel"] = {
            DoorPoint(FungalHall_Phazon,Up),
            DoorPoint(Phazon_FungalHall,Down)
        };
        points["Fungal Hall Access"] = {
            DoorPoint(FungalHall_ElevatorB,Up),
            DoorPoint(ElevatorB_FungalHall,Down)
        };
        points["Fungal Hall A"] = {
            DoorPoint(Phazon_FungalHall,Up),
            DoorPoint(FungalHall_ElevatorB,Down)
        };
    }

    RoomDoorList worldmap = preset->untouched_doors.at(world);
    RoomDoorList exclumap = preset->excluded_doors.at(world);

    for(auto room = points.begin();room!=points.end();++room) {
        QString room_name = room->first;
        vector<DoorPoint> c_room = room->second;
        for (auto door = c_room.begin();door!=c_room.end();++door) {
           auto door_index = door-c_room.begin();
           DoorPoint point = points.at(room_name).at(door_index);
           DoorMapIcon *icon = new DoorMapIcon(this,world,room_name,door_index,point,worldmap[room_name][door_index]);
           icon->exclude(exclumap[room_name][door_index]);
           map->scene()->addItem(icon);
        }
    }
}

void DoorExcluder::excludeDoor(World world,QString room, int index, bool exclude) {
    excluded_doors.at(world)[room][index] = exclude;
}

void DoorExcluder::on_buttonBox_accepted()
{
    preset->excluded_doors.swap(excluded_doors);
}
