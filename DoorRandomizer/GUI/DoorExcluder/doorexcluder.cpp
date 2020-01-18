#include <doormapicon.h>

#include "doorexcluder.h"
#include "ui_doorexcluder.h"

DoorExcluder::DoorExcluder(QWidget *parent, Preset *main_preset)
    : QDialog(parent)
    , ui(new Ui::DoorExcluder)
{
    ui->setupUi(this);
    preset = main_preset;
    QString mapimg_path = QCoreApplication::applicationDirPath().append("/resources/maps/");
    ui->label_DoorError->setStyleSheet("QLabel { color:transparent; }");

    initializeExcludedDoors();

    initializeMap(ui->map_tallon,QPixmap(mapimg_path + "tallon_map.jpg"));
    initializeMap(ui->map_chozo,QPixmap(mapimg_path + "chozo_map.jpg"));
    initializeMap(ui->map_magmoor,QPixmap(mapimg_path + "magmoor_map.jpg"));
    initializeMap(ui->map_phendrana,QPixmap(mapimg_path + "phendrana_map.jpg"));
    initializeMap(ui->map_mines,QPixmap(mapimg_path + "mines_map.jpg"));

    addIcons(ui->map_tallon,TallonOverworld);
    addIcons(ui->map_chozo,ChozoRuins);
}

DoorExcluder::~DoorExcluder()
{
    delete ui;
    delete excluded_doors;
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

void DoorExcluder::initializeExcludedDoors() {

    excluded_doors = new vector<RoomDoorList>();

    RoomDoorList tallon;
    RoomDoorList chozo;
    RoomDoorList magmoor;
    RoomDoorList phendrana;
    RoomDoorList mines;

    chozo["Transport to Tallon Overworld North"] = {false};
    chozo["Ruins Entrance"] = {false,false};
    chozo["Main Plaza"] = {false,false,false,false,true,true};
    chozo["Ruined Fountain Access"] = {false,false};
    chozo["Ruined Shrine Access"] = {false,false};
    chozo["Nursery Access"] = {false,false};
    chozo["Plaza Access"] = {false,false};
    chozo["Ruined Fountain"] = {false,false,false};
    chozo["Ruined Shrine"] = {false,false};
    chozo["Eyon Tunnel"] = {false,false};
    chozo["Vault"] = {false,false};
    chozo["Training Chamber"] = {false,true};
    chozo["Arboretum Access"] = {false,false};
    chozo["Meditation Fountain"] = {false,false};
    chozo["Tower of Light Access"] = {false,false};
    chozo["Ruined Nursery"] = {false,false,false};
    chozo["Vault Access"] = {false,false};
    chozo["Training Chamber Access"] = {false,false};
    chozo["Arboretum"] = {false,false,false};
    chozo["Magma Pool"] = {false,false};
    chozo["Tower of Light"] = {false,true};
    chozo["Save Station 1"] = {false};
    chozo["North Atrium"] = {false,false};
    chozo["Transport to Magmoor Caverns North"] = {false,false,false};
    chozo["Sunchamber Lobby"] = {false,false};
    chozo["Gathering Hall Access"] = {false,false};
    chozo["Tower Chamber"] = {true};
    chozo["Ruined Gallery"] = {false,false,false};
    chozo["Sun Tower"] = {false,false};
    chozo["Transport Access North"] = {false,false};
    chozo["Sunchamber Access"] = {false,false};
    chozo["Gathering Hall"] = {false,false,false,false};
    chozo["Totem Access"] = {false,false};
    chozo["Map Station"] = {false};
    chozo["Sun Tower Access"] = {false,false};
    chozo["Hive Totem"] = {false,false};
    chozo["Sunchamber"] = {false,false};
    chozo["Watery Hall Access"] = {false,false};
    chozo["Save Station 2"] = {false};
    chozo["East Atrium"] = {false,false};
    chozo["Watery Hall"] = {false,false};
    chozo["Energy Core Access"] = {false,false};
    chozo["Dynamo Access"] = {false,false};
    chozo["Energy Core"] = {true,false,false};
    chozo["Dynamo"] = {false};
    chozo["Burn Dome Access"] = {false,true};
    chozo["West Furnace Access"] = {false,false};
    chozo["Burn Dome"] = {false};
    chozo["Furnace"] = {false,false,true};
    chozo["East Furnace Access"] = {false,false};
    chozo["Crossway Access West"] = {false,true};
    chozo["Hall of the Elders"] = {false,false,false,false,true};
    chozo["Crossway"] = {false,false,false};
    chozo["Reflecting Pool Access"] = {false,false};
    chozo["Elder Hall Access"] = {false,false};
    chozo["Crossway Access South"] = {false,false};
    chozo["Elder Chamber"] = {true};
    chozo["Reflecting Pool"] = {false,false,false,false};
    chozo["Save Station 3"] = {false,false};
    chozo["Transport Access South"] = {false,false};
    chozo["Antechamber"] = {false};
    chozo["Transport to Tallon Overworld East"] = {false};
    chozo["Transport to Tallon Overworld South"] = {false};
    chozo["Piston Hall"] = {true,true};
    phendrana["Transport to Magmoor Caverns West"] = {false};
    phendrana["Shoreline Entrance"] = {false,false};
    phendrana["Phendrana Shorelines"] = {false,false,false,false,false,false};
    phendrana["Temple Entryway"] = {false,false};
    phendrana["Shoreline Entrance"] = {false,false};
    phendrana["Save Station B"] = {false};
    phendrana["Ruins Entryway"] = {false,false};
    phendrana["Plaza Walkway"] = {false,false};
    phendrana["Ice Ruins Access"] = {false,false};
    phendrana["Chozo Ice Temple"] = {false,false};
    phendrana["Ice Ruins West"] = {false,false};
    phendrana["Ice Ruins East"] = {false,false};
    phendrana["Chapel Tunnel"] = {false,false};
    phendrana["Canyon Entryway"] = {false,false};
    phendrana["Chapel of the Elders"] = {false};
    phendrana["Ruined Courtyard"] = {false,false,false,false};
    phendrana["Phendrana Canyon"] = {false};
    phendrana["Save Station A"] = {false};
    phendrana["Specimen Storage"] = {false,false};
    phendrana["Quarantine Access"] = {false,false};
    phendrana["Research Entrance"] = {false,false,false};
    phendrana["North Quarantine Tunnel"] = {false,false,false,false};
    phendrana["Map Station"] = {false};
    phendrana["Quarantine Cave"] = {false,false,true};
    phendrana["Research Lab Hydra"] = {false,true};
    phendrana["South Quarantine Tunnel"] = {false,false};
    phendrana["Quarantine Monitor"] = {true};
    phendrana["Observatory Access"] = {false,true};
    phendrana["Transport to Magmoor Caverns South"] = {false,false};
    phendrana["Observatory"] = {false,false,false};
    phendrana["Transport Access"] = {false,false};
    phendrana["West Tower Entrance"] = {false,false};
    phendrana["Save Station D"] = {false};
    phendrana["Frozen Pike"] = {false,false,false,false};
    phendrana["West Tower "] = {false,false}; //todo: Door_index in all of these is 0?
    phendrana["Pike Access"] = {false,false};
    phendrana["Frost Cave Access"] = {false,false};
    phendrana["Hunter Cave Access"] = {false,false};
    phendrana["Control Tower"] = {false,false};
    phendrana["Research Core"] = {false,false};
    phendrana["Frost Cave"] = {false,false,false};
    phendrana["Hunter Cave"] = {false,false,false,false};
    phendrana["Research Core Access"] = {false,true};
    phendrana["Save Station C"] = {false};
    phendrana["Upper Edge Tunnel"] = {false,false};
    phendrana["Lower Edge Tunnel"] = {false,false};
    phendrana["Chamber Access"] = {false,false};
    phendrana["Lake Tunnel"] = {false,false};
    phendrana["Aether Lab Entryway"] = {false,false};
    phendrana["Research Lab Aether"] = {false,true};
    phendrana["Phendrana's Edge"] = {false,false,false};
    phendrana["Gravity Chamber"] = {false,false};
    phendrana["Storage Cave"] = {false};
    phendrana["Security Cave"] = {true};
    tallon["Landing Site"] = {false,false,false,false,false};
    tallon["Gully"] = {false,false};
    tallon["Canyon Cavern"] = {false,false};
    tallon["Temple Hall"] = {false,false};
    tallon["Alcove"] = {false};
    tallon["Waterfall Cavern"] = {false,false};
    tallon["Tallon Canyon"] = {false,false,false,false};
    tallon["Temple Security Station"] = {false,false};
    tallon["Frigate Crash Site"] = {false,false,false};
    tallon["Transport Tunnel A"] = {false,false};
    tallon["Root Tunnel"] = {false,false};
    tallon["Temple Lobby"] = {false,false};
    tallon["Frigate Access Tunnel"] = {false,false};
    tallon["Overgrown Cavern"] = {false,false};
    tallon["Transport to Chozo Ruins West"] = {false};
    tallon["Root Cave"] = {false,false,false};
    tallon["Artifact Temple"] = {false};
    tallon["Main Ventilation Shaft Section C"] = {true,false};
    tallon["Main Ventilation Shaft Section B"] = {true,true,true,true};
    tallon["Main Ventilation Shaft Section A"] = {true,true};
    tallon["Transport Tunnel C"] = {false,false};
    tallon["Transport Tunnel B"] = {false,false};
    tallon["Arbor Chamber"] = {false};
    tallon["Transport to Chozo Ruins East"] = {false};
    tallon["Transport to Magmoor Caverns East"] = {false};
    tallon["Reactor Core"] = {true,true};
    tallon["Reactor Access"] = {true,true,false};
    tallon["Cargo Freight Lift to Deck Gamma"] = {true,false};
    tallon["Savestation"] = {false};
    tallon["Deck Beta Transit Hall"] = {false,false};
    tallon["Biohazard Containment"] = {false,false};
    tallon["Deck Beta Security Hall"] = {false,false};
    tallon["Biotech Research Area 1"] = {false,false};
    tallon["Deck Beta Conduit Hall"] = {false,false};
    tallon["Connection Elevator to Deck Beta"] = {false,false};
    tallon["Hydro Access Tunnel"] = {false,false};
    tallon["Great Tree Hall"] = {false,false,false,false,false};
    tallon["Great Tree Chamber"] = {false};
    tallon["Transport Tunnel D"] = {false,false};
    tallon["Life Grove Tunnel"] = {false,true};
    tallon["Transport Tunnel E"] = {false,false};
    tallon["Transport to Chozo Ruins South"] = {false};
    tallon["Life Grove"] = {true};
    tallon["Transport to Phazon Mines East"] = {false};
    mines["Quarry Access"] = {false,false};
    mines["Main Quarry"] = {false,false,false,false};
    mines["Waste Disposal"] = {false,false};
    mines["Save Station Mines A"] = {false};
    mines["Security Access A"] = {false,false};
    mines["Ore Processing"] = {false,false,false,false};
    mines["Mine Security Station"] = {false,true,false};
    mines["Research Access"] = {false,false};
    mines["Storage Depot B"] = {false};
    mines["Elevator Access A"] = {false,false};
    mines["Storage Depot A"] = {false};
    mines["Elite Research"] = {false,true};
    mines["Elevator A"] = {false,false};
    mines["Elite Control Access"] = {false,false};
    mines["Elite Control"] = {false,false,true};
    mines["Maintenance Tunnel"] = {false,false};
    mines["Ventilation Shaft"] = {false,true};
    mines["Phazon Processing Center"] = {false,false,false};
    mines["Omega Research"] = {false,false,true};
    mines["Transport Access"] = {false,false};
    mines["Processing Center Access"] = {false,false};
    mines["Map Station Mines"] = {false};
    mines["Dynamo Access"] = {false,true};
    mines["Transport to Magmoor Caverns South"] = {false};
    mines["Elite Quarters"] = {false,false};
    mines["Central Dynamo"] = {false,false,false};
    mines["Elite Quarters Access"] = {false,false};
    mines["Quarantine Access A"] = {false,false};
    mines["Save Station Mines B"] = {false};
    mines["Metroid Quarantine B"] = {false,false,false};
    mines["Metroid Quarantine A"] = {false,false};
    mines["Quarantine Access B"] = {false,false};
    mines["Save Station Mines C"] = {false};
    mines["Elevator Access B"] = {false,false};
    mines["Fungal Hall B"] = {false,false,false};
    mines["Elevator B"] = {false,false};
    mines["Missile Station Mines"] = {false};
    mines["Phazon Mining Tunnel"] = {false,false};
    mines["Fungal Hall Access"] = {false,false};
    mines["Fungal Hall A"] = {false,false};
    magmoor["Transport to Chozo Ruins North"] = {false};
    magmoor["Burning Trail"] = {false,false,false};
    magmoor["Lake Tunnel"] = {false,false};
    magmoor["Save Station Magmoor A"] = {false};
    magmoor["Lava Lake"] = {false,false};
    magmoor["Pit Tunnel"] = {false,false};
    magmoor["Triclops Pit"] = {false,false,false};
    magmoor["Monitor Tunnel"] = {false,false};
    magmoor["Storage Cavern"] = {false};
    magmoor["Monitor Station"] = {false,false,false,false};
    magmoor["Transport Tunnel A"] = {false,false};
    magmoor["Warrior Shrine"] = {false,true};
    magmoor["Shore Tunnel"] = {false,false};
    magmoor["Transport to Phendrana Drifts North"] = {false};
    magmoor["Fiery Shores"] = {false,false};
    magmoor["Transport Tunnel B"] = {false,false};
    magmoor["Transport to Tallon Overworld West"] = {false,false};
    magmoor["Twin Fires Tunnel"] = {false,false};
    magmoor["Twin Fires"] = {false,false};
    magmoor["North Core Tunnel"] = {false,false};
    magmoor["Geothermal Core"] = {false,false,false};
    magmoor["Plasma Processing"] = {false};
    magmoor["South Core Tunnel"] = {false,false};
    magmoor["Magmoor Workstation"] = {false,false,false};
    magmoor["Workstation Tunnel"] = {false,false};
    magmoor["Transport Tunnel C"] = {false,false};
    magmoor["Transport to Phazon Mines West"] = {false};
    magmoor["Transport to Phendrana Drifts South"] = {false,false};
    magmoor["Save Station Magmoor B"] = {false,false};


    excluded_doors->push_back(tallon);
    excluded_doors->push_back(chozo);
    excluded_doors->push_back(magmoor);
    excluded_doors->push_back(phendrana);
    excluded_doors->push_back(mines);
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

    } else if (world == PhendranaDrifts) {

    } else if (world == PhazonMines) {

    }

    RoomDoorList worldmap = excluded_doors->at(world);

    for(auto room = points.begin();room!=points.end();++room) {
        QString room_name = room->first;
        vector<DoorPoint> c_room = room->second;
        for (auto door = c_room.begin();door!=c_room.end();++door) {
           auto door_index = door-c_room.begin();
           DoorPoint point = points.at(room_name).at(door_index);
           map->scene()->addItem(new DoorMapIcon(this,world,room_name,door_index,point,worldmap[room_name][door_index]));
        }
    }
}
