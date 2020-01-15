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
    chozo["Main Plaza"] = {false,false,false,false,true};
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
    chozo["Hive Totem"] = {false,false};
    chozo["Watery Hall Access"] = {false,false};
    chozo["Save Station 2"] = {false};
    chozo["East Atrium"] = {false,false};
    chozo["Watery Hall"] = {false,false};
    chozo["Energy Core Access"] = {false,false};
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

    const bool UP = false;
    const bool LEFT = false;
    const bool DOWN = true;
    const bool RIGHT = true;


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
            DoorPoint(Landing_Gully,RIGHT,Vertical),
            DoorPoint(Landing_Canyon,RIGHT,Vertical),
            DoorPoint(Landing_Temple,UP,Horizontal),
            DoorPoint(Landing_Alcove,DOWN,Horizontal),
            DoorPoint(Landing_Waterfall,LEFT,Vertical)
        };
        points["Gully"] = {
            DoorPoint(Gully_Cannon,RIGHT,Vertical),
            DoorPoint(Landing_Gully,LEFT,Vertical)
        };
        points["Canyon Cavern"] = {
            DoorPoint(Canyon_Cavern,RIGHT,Vertical),
            DoorPoint(Landing_Canyon,LEFT,Vertical)
        };
        points["Temple Hall"] = {
            DoorPoint(Temple_Monitor,UP,Horizontal),
            DoorPoint(Landing_Temple,DOWN,Horizontal)
        };
        points["Alcove"] = {
            DoorPoint(Landing_Alcove,UP,Horizontal)
        };
        points["Waterfall Cavern"] = {
            DoorPoint(Landing_Waterfall,RIGHT,Vertical),
            DoorPoint(Crash_Waterfall,LEFT,Vertical)
        };
        points["Tallon Canyon"] = {
            DoorPoint(Canyon_Cavern,LEFT,Vertical),
            DoorPoint(Canyon_ChozoHall,DOWN,Horizontal),
            DoorPoint(Gully_Cannon,LEFT,Vertical),
            DoorPoint(Canyon_Roots,RIGHT,Vertical)
        };
        points["Temple Security Station"] = {
            DoorPoint(Temple_Monitor,DOWN,Horizontal),
            DoorPoint(Monitor_TempleHall,UP,Horizontal)
        };
        points["Frigate Crash Site"] = {
            DoorPoint(Crash_Waterfall,RIGHT,Vertical),
            DoorPoint(Crash_Entrance,LEFT,Vertical),
            DoorPoint(Crash_Elevator,DOWN,Horizontal)
        };
        points["Transport Tunnel A"] = {
            DoorPoint(Chozo_CanyonHall,DOWN,Horizontal),
            DoorPoint(Canyon_ChozoHall,UP,Horizontal)
        };
        points["Root Tunnel"] = {
            DoorPoint(Root_Hall,RIGHT,Vertical),
            DoorPoint(Canyon_Roots,LEFT,Vertical)
        };
        points["Temple Lobby"] = {
            DoorPoint(Temple_Lobby,UP,Horizontal),
            DoorPoint(Monitor_TempleHall,DOWN,Horizontal)
        };
        points["Frigate Access Tunnel"] = {
            DoorPoint(Crash_VenC,LEFT,Vertical),
            DoorPoint(Crash_Entrance,RIGHT,Vertical)
        };
        points["Overgrown Cavern"] = {
            DoorPoint(Crash_Elevator,UP,Horizontal),
            DoorPoint(Overgrown_Tree,DOWN,Horizontal)
        };
        points["Transport to Chozo Ruins West"] = {
            DoorPoint(Chozo_CanyonHall,UP,Horizontal)
        };
        points["Root Cave"] = {
            DoorPoint(Root_MagmoorHall,UP,Horizontal),
            DoorPoint(Root_Hall,LEFT,Vertical),
            DoorPoint(Root_Arbor,RIGHT,Vertical)
        };
        points["Artifact Temple"] = {
            DoorPoint(Temple_Lobby,DOWN,Horizontal)
        };
        points["Main Ventilation Shaft Section C"] = {
            DoorPoint(VenC_VenB,UP,Horizontal),
            DoorPoint(Crash_VenC,RIGHT,Vertical)
        };
        points["Main Ventilation Shaft Section B"] = {
            DoorPoint(VenB_VenA,UP,Horizontal),
            DoorPoint(VenC_VenB,DOWN,Horizontal),
            DoorPoint(VenB_VenB,UP,Horizontal),
            DoorPoint(VenB_VenB,DOWN,Horizontal)
        };
        points["Main Ventilation Shaft Section A"] = {
            DoorPoint(VenB_VenA,DOWN,Horizontal),
            DoorPoint(VenA_Core,UP,Horizontal)
        };
        points["Transport Tunnel C"] = {
            DoorPoint(Overgrown_Tree,UP,Horizontal),
            DoorPoint(Overgrown_Elevator,LEFT,Vertical)
        };
        points["Transport Tunnel B"] = {
            DoorPoint(Root_MagmoorHall,DOWN,Horizontal),
            DoorPoint(MagmoorHall_Elevator,RIGHT,Vertical)
        };
        points["Arbor Chamber"] = {
            DoorPoint(Root_Arbor,LEFT,Vertical)
        };
        points["Transport to Chozo Ruins East"] = {
            DoorPoint(Overgrown_Elevator,RIGHT,Vertical)
        };
        points["Transport to Magmoor Caverns East"] = {
            DoorPoint(MagmoorHall_Elevator,LEFT,Vertical)
        };
        points["Reactor Core"] = {
            DoorPoint(ReactorCore_Access,RIGHT,Vertical),
            DoorPoint(VenA_Core,DOWN,Horizontal)
        };
        points["Reactor Access"] = {
                DoorPoint(ReactorAccess_Lift,RIGHT,Vertical),
                DoorPoint(ReactorCore_Access,LEFT,Vertical),
                DoorPoint(ReactorAccess_Save,DOWN,Horizontal),
        };
        points["Cargo Freight Lift to Deck Gamma"] = {
                DoorPoint(ReactorAccess_Lift,LEFT,Vertical),
                DoorPoint(CargoLift_Hall,UP,Horizontal)
        };
        points["Savestation"] = {
                DoorPoint(ReactorAccess_Save,UP,Horizontal)
        };
        points["Deck Beta Transit Hall"] = {
                DoorPoint(CargoLift_Hall,DOWN,Horizontal),
                DoorPoint(Biohazard_Transit,UP,Horizontal)
        };
        points["Biohazard Containment"] = {
                DoorPoint(Biohazard_Transit,DOWN,Horizontal),
                DoorPoint(Biohazard_Security,LEFT,Vertical)
        };
        points["Deck Beta Security Hall"] = {
                DoorPoint(Biohazard_Security,RIGHT,Vertical),
                DoorPoint(Biotech_Security,UP,Horizontal)
        };
        points["Biotech Research Area 1"] = {
                DoorPoint(Biotech_Security,DOWN,Horizontal),
                DoorPoint(Biotech_Conduit,UP,Horizontal)
        };
        points["Deck Beta Conduit Hall"] = {
                DoorPoint(Beta_Conduit,RIGHT,Vertical),
                DoorPoint(Biotech_Conduit,DOWN,Horizontal)
        };
        points["Connection Elevator to Deck Beta"] = {
                DoorPoint(Beta_Conduit,LEFT,Vertical),
                DoorPoint(Beta_Hydro,UP,Horizontal)
        };
        points["Hydro Access Tunnel"] = {
                DoorPoint(Tree_Frigate,LEFT,Vertical),
                DoorPoint(Beta_Hydro,DOWN,Horizontal)
        };
        points["Great Tree Hall"] = {
                DoorPoint(Tree_Frigate,RIGHT,Vertical),
                DoorPoint(Tree_Chamber,LEFT,Vertical),
                DoorPoint(Tree_ElevatorD,DOWN,Horizontal),
                DoorPoint(Tree_Grove,UP,Horizontal),
                DoorPoint(Tree_ElevatorE,UP,Horizontal)
        };
        points["Great Tree Chamber"] = {
                DoorPoint(Tree_Chamber,RIGHT,Vertical)
        };
        points["Transport Tunnel D"] = {
                DoorPoint(Tree_Chozo,LEFT,Vertical),
                DoorPoint(Tree_ElevatorD,UP,Horizontal)
        };
        points["Life Grove Tunnel"] = {
                DoorPoint(Tree_Grove,DOWN,Horizontal),
                DoorPoint(Grove_Tunnel,UP,Horizontal)
        };
        points["Transport Tunnel E"] = {
                DoorPoint(Tree_Phazon,RIGHT,Vertical),
                DoorPoint(Tree_ElevatorE,DOWN,Horizontal)
        };
        points["Transport to Chozo Ruins South"] = {
                DoorPoint(Tree_Chozo,RIGHT,Vertical)
        };
        points["Life Grove"] = {
                DoorPoint(Grove_Tunnel,DOWN,Horizontal)
        };
        points["Transport to Phazon Mines East"] = {
                DoorPoint(Tree_Phazon,LEFT,Vertical)
        };
    } else if (world == ChozoRuins) {

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
