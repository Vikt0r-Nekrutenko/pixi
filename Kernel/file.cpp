#include "file.hpp"
#include <iostream>

pixi::files::file::file(const std::string &fileName)
{
    m_dataFile = fopen(fileName.c_str(), "rb");
}

pixi::files::file::~file()
{
    fclose(m_dataFile);
}

std::string pixi::files::file::field(const size_t &indx, const size_t &fieldNumber)
{
    fseek(m_dataFile, FIELD_SIZE * 6 * indx + FIELD_SIZE * fieldNumber, SEEK_SET);
    char field[FIELD_SIZE];
    fread(field, FIELD_SIZE, 1, m_dataFile);
    return field;
}

std::vector<pixi::files::byte> pixi::files::file::signature(const size_t &indx)
{
    fseek(m_dataFile, FIELD_SIZE * 6 * indx + FIELD_SIZE * 5, SEEK_SET);
    std::vector<byte> signature(SIGNATURE_SIZE);
    for (word i = 0; i < SIGNATURE_SIZE; i++) {
        fread(&signature[i], 1, 1, m_dataFile);
    }
    return signature;
}

/*while(!softwareFile.eof())
{
    vector<string> fields(6);
    for (int i = 0; i < 6; i++) {
        softwareFile >> fields[i];
    }

    if (fields[5] == "undefined") continue;

    for (int i = 0; i < 5; i++)
    {
        unsigned char binField[256]; memset(binField, 0, 256);
        for (size_t k = 0; k < fields[i].length(); k++) binField[k] = fields[i][k];
        fwrite(binField, 256, 1, softwareDataSet);
    }

    for (size_t i = 2; i < fields[5].size(); i+=3) {
        fields[5].insert(i, " ");
    }

    istringstream stream(fields[5]);
    unsigned char signatureField[256]; memset(signatureField, 0, 256);

    int i = 0;
    while(!stream.eof()) {
        int tmp = 0;
        stream >> hex >> tmp; signatureField[i++] = tmp;
    }

    fwrite(signatureField, 256, 1, softwareDataSet);
}*/
