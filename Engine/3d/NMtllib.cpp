#include "NMtllib.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include "NTexture.h"

NMaterial NMtllib::Load(const std::string& directoryPath, const std::string& filename)
{
    NMtllib data;
    NMaterial material;
    material.texture = NTextureManager::GetInstance()->CreateErrorTexture();

    //�t�@�C���X�g���[��
    std::ifstream file;
    //�}�e���A����񏑂��Ă�t�@�C�����J��
    file.open((directoryPath + filename));
    //�t�@�C���J���Ȃ�������G���[
    if (file.fail()) {
        assert(0);
    }

    //��s���ǂݍ���
    std::string line;
    while (getline(file, line)) {
        //1�s���̕�������X�g���[���ɕϊ�
        std::istringstream line_stream(line);

        //���p�X�y�[�X��؂�ōs�̐擪��������擾
        std::string key;
        getline(line_stream, key, ' ');

        //�擪�̃^�u�����͖�������
        if (key[0] == '\t') {
            key.erase(key.begin()); //�擪�̕������폜
        }
        //�擪�̕�����newmtl�Ȃ�}�e���A����
        if (key == "newmtl") {
            //�}�e���A�����ǂݍ���
            line_stream >> material.name;
        }
        //�擪�̕�����Ka�Ȃ�A���r�G���g�F(����)
        if (key == "Ka") {
            line_stream >> material.ambient.x;
            line_stream >> material.ambient.y;
            line_stream >> material.ambient.z;
        }
        //�擪�̕�����Kd�Ȃ�f�B�t���[�Y�F(�g�U���ˌ�)
        if (key == "Kd") {
            line_stream >> material.diffuse.x;
            line_stream >> material.diffuse.y;
            line_stream >> material.diffuse.z;
        }
        //�擪�̕�����Ks�Ȃ�X�y�L�����[�F(���ʔ��ˌ�)
        if (key == "Ks") {
            line_stream >> material.specular.x;
            line_stream >> material.specular.y;
            line_stream >> material.specular.z;
        }
        //�擪�̕�����map_Kd�Ȃ�e�N�X�`���t�@�C����
        if (key == "map_Kd") {
            //�e�N�X�`���̃t�@�C�����ǂݍ���
            line_stream >> material.textureFilename;
            std::string texFileName = (directoryPath + material.textureFilename);
            material.texture = NTextureManager::GetInstance()->LoadTexture(texFileName, material.textureFilename);
        }
    }
    file.close();

    //�Ō�ɂ����Ɠǂݍ��񂾏��V�F�[�_�[�ɗ�����
    material.TransferLight();
    return material;
}
