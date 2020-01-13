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

    initializeMap(ui->map_tallon,QPixmap(mapimg_path + "tallon_map.jpg"));
    initializeMap(ui->map_chozo,QPixmap(mapimg_path + "chozo_map.jpg"));
    initializeMap(ui->map_magmoor,QPixmap(mapimg_path + "magmoor_map.jpg"));
    initializeMap(ui->map_phendrana,QPixmap(mapimg_path + "phendrana_map.jpg"));
    initializeMap(ui->map_mines,QPixmap(mapimg_path + "mines_map.jpg"));

    vector<DoorMapIcon*> icons;

    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,0,0,QPoint(728,570),true,DoorOrientation::Horizontal));
    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,33,0,QPoint(728,570),false,DoorOrientation::Horizontal));
    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,0,1,QPoint(618,582),true,DoorOrientation::Vertical));
    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,6,1,QPoint(618,582),false,DoorOrientation::Vertical));
    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,6,1,QPoint(673,577),false,DoorOrientation::Floor));
    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,0,1,QPoint(1215,657),true,DoorOrientation::Vertical,true));
    icons.push_back(new DoorMapIcon(this,World::TallonOverworld,25,0,QPoint(1215,657),false,DoorOrientation::Vertical,true));

    for(auto i=icons.begin();i!=icons.end();++i)
        ui->map_tallon->scene()->addItem(*i);
}

DoorExcluder::~DoorExcluder()
{
    delete ui;
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
    ui->lineEdit->setText(QString("World: %1, Area: %2").arg(selected_icon->world()).arg(selected_icon->areaID()));
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
